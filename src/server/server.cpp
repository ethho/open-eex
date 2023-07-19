#include "server.hpp"

Server::Server(): rs(2) {}

void Server::add_order(const Order& o){
    this->m[o.symbol()].addOrder(o);
    this->run_matching_engine(o.symbol());
}


void Server::add_bid(std::string ticker, double price_per_share, double volume, Client *c){
    Bid b(ticker, price_per_share, volume);
    b.set_client_ptr(c);
    this->m[ticker].addOrder(b);
    this->run_matching_engine(ticker);
}

void Server::add_ask(std::string ticker, double price_per_share, double volume, Client *c){
    Ask a(ticker, price_per_share, volume);
    a.set_client_ptr(c);
    this->m[ticker].addOrder(a);
    this->run_matching_engine(ticker);
}

void Server::create_server(const char* port_p, int buffer_size, int backlog){

    //using the struct key word because the header files are native to C 
    //we do not want overlap with any other type with the same name in the global namespace
    struct addrinfo hints;  //this variable will hold the hints required to the call to getaddrinfo()
    struct addrinfo* p;     //iterator for nodes of list returned by getaddrinfo()
    struct addrinfo* res;   //head of list returned by getaddrinfo()

    struct sockaddr_storage client_addr;    //placeholder for client_addr
    socklen_t addr_size = sizeof (struct sockaddr_storage);                    //size of client addr placeholder

    int active_socket_fd;   //file descriptor for active socket

    int ret;    //placeholder for return value

    Client *c;

    char *client_name;
    std::thread t;

    //create a copy of the port name
    this->port = new char[std::strlen(port_p)];
    std::strcpy(this->port, port_p);

    std::memset(&hints, 0, sizeof (hints));
    hints.ai_family = AF_UNSPEC;        //do not specify whether IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM;    //socket stream as opposed to socket_datagram
    hints.ai_flags = AI_PASSIVE;        //fill my IP address for me

    this->buffer_size = buffer_size;
    this->backlog = backlog;

    //first param is NULL because domain name is unspecified, plus hints.ai_flags is passive
    ret = getaddrinfo(NULL, this->port, &hints, &res);
    if(ret != 0){
        std::printf("Call to getaddinfo failed! Value of errno = %d, exiting function!\n", errno);
        return;
    }

    //get the server hostname
    this->server_hostname = new char[this->buffer_size];
    ret = gethostname(this->server_hostname, this->buffer_size);
    if(ret == -1){
        std::printf("Could not get server hostname, value of errno = %d, exiting function!\n", errno);
        return;
    }
    std::printf("Welcome to Open-EEX, server name is %s, port is %s\n", this->server_hostname, this->port);

    for(p = res; p != nullptr; p = p->ai_next){
        this->passive_socket_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if(this->passive_socket_fd < 0){
            std::printf("Call to create socket failed, value of errno = %d\n", errno);
            continue;
        }

        ret = bind(this->passive_socket_fd, p->ai_addr, p->ai_addrlen);
        if(ret < 0){
            std::printf("Call to bind failed, value of errno = %d\n", errno);
            close(this->passive_socket_fd);
            continue;
        }

        ret = listen(this->passive_socket_fd, this->backlog);
        if(ret < 0){
            std::printf("Call to listen failed, value of errno = %d\n", errno);
            close(this->passive_socket_fd);
            continue;
        }
        break;
    }

    if(p == nullptr){
        std::printf("No available socket for the server to listen with, returning from function\n");
        return;
    }

    //we do not need the linkedlist now, the server is listening on this->passive_socket
    freeaddrinfo(res);

    //create an infinite loop and keep accepting clients
    while(1){
        active_socket_fd = accept(this->passive_socket_fd, (struct sockaddr*) &client_addr, &addr_size);
        if(active_socket_fd < 0){
            //no one requested to connect
        }else{
            //someone has requested to connect
            //prepare to handle the client here
            c = new Client();
            c->create_client(this,active_socket_fd,this->buffer_size);
            client_name = c->get_client_hostname_ptr();
            ret = getnameinfo((struct sockaddr *)&client_addr, sizeof(client_addr), client_name, this->buffer_size, this->server_hostname, this->buffer_size, 0);
            if (ret == -1){
                std::printf("Could not get client name, skipping client\n");
                continue;
            }

            this->all_threads.push_back(std::thread(&Server::handle_client,this,c));
            //this->all_threads.push_back(t);
        }
    }
    int n_clients = this->all_threads.size();
    for(int i=0;i<n_clients;i++){
        this->all_threads[i].join();
    }
}

void Server::create_order(OrderPacket* o, Client* c){
    if(o->true_if_bid){
        this->rs.launch_task(&Server::add_bid, this, o->ticker, o->price_per_share, o->num_shares,c);
        return;
    }else{
        //Volume must be negative for Asks
        this->rs.launch_task(&Server::add_ask, this, o->ticker, o->price_per_share, -o->num_shares,c);
        return;
    }
}

void send_all(int sockfd, char* buf_ptr, int length, int flags){
    int n_bytes=0;
    int bytes_sent;
    while(n_bytes < length){
        bytes_sent = send(sockfd,buf_ptr, length, flags);
        n_bytes = n_bytes + bytes_sent;
        buf_ptr = buf_ptr + bytes_sent;
    }
    return;
}

void Server::run_matching_engine(std::string ticker){
    Bid bid_order;
    Ask ask_order;
    MatchingEngine *eng = &(this->m[ticker]);
    Client* bidder_ptr;
    Client* asker_ptr;
    double vol_rem, price_used;
    
    //if there are both buy and sell orders only then can we check for a match 
    std::printf("Running Orders\n");
    if(eng->any_orders()){
        std::printf("We got em\n");
        bid_order = eng->top_buy();
        ask_order = eng->top_sell();
        
        //If orders are compatible, this function will also modify the two orders, calling by reference
        if(compatible_orders(bid_order,ask_order, vol_rem, price_used)){
            //We will have to pop from the heap as well
            eng->pop_buy();
            eng->pop_sell();
            //Add back to the heap if the order did not settle it.
            //We could have modified the heaps in place, but the C++ stl does not allow that I think 
            if(bid_order.volume() != 0){
                std::printf("Adding bid, volume = %f", bid_order.volume());
                eng->addOrder(bid_order);
            }
            if(ask_order.volume() != 0){
                std::printf("Adding ask, volume = %f", ask_order.volume());
                eng->addOrder(ask_order);
            }

            std::printf("Got rid of all orders, %d\n", eng->any_orders());

            //Send the bid to the bidder's client and the ask to the asker's client
            bidder_ptr = bid_order.get_client_ptr();
            asker_ptr = ask_order.get_client_ptr();

            //prepare the buffers for sending and send, and the client is responsible for updating the portfolio
            std::sprintf(asker_ptr->server_buffer,"%s:ASK:%f:%f\r\n",ticker.c_str(),price_used, ask_order.volume()); 
            send_all(asker_ptr->active_socket_fd, asker_ptr->server_buffer, strlen(asker_ptr->server_buffer), 0);  
            std::memset(asker_ptr->server_buffer, 0, asker_ptr->buffer_size);

            std::sprintf(bidder_ptr->server_buffer,"%s:BID:%f:%f\r\n",ticker.c_str(),price_used, bid_order.volume());
            send_all(bidder_ptr->active_socket_fd, bidder_ptr->server_buffer, strlen(bidder_ptr->server_buffer), 0);            
            std::memset(bidder_ptr->server_buffer, 0, bidder_ptr->buffer_size);
              
        }  
    }
}

void Server::run_matching_engine(){
    //MatchingEngine *eng;    //Use a pointer because the implicit = operator for MatchingEngine has been deleted
    std::string ticker_name;
    Bid bid_order;
    Ask ask_order;
    //Client* bidder_ptr;
    //Client* asker_ptr;
    //iterate through the whole map with the iterator
    std::unordered_map<std::string,MatchingEngine>::iterator it = this->m.begin();
    while(it != this->m.end()){
        //check whether an order can be executed here
        ticker_name = it->first;
        //eng = &(it->second);
        std::cout << ticker_name << std::endl;
        //bid_order = eng->top_buy();
        //ask_order = eng->top_sell();

        //If orders are compatible, this function will also modify the two orders, calling by reference
        /*
        if(compatible_orders(bid_order,ask_order)){
            std::printf("Match Made!\n");
            //Send the bid to the bidder's client and the ask to the asker's client
            bidder_ptr = bid_order.get_client_ptr();
            asker_ptr = ask_order.get_client_ptr();

            //prepare the buffers for sending
            std::sprintf(bidder_ptr->server_buffer,"%s:%s:%f:%f",ask_order.symbol().c_str(),ask_order.typeName().c_str(), ask_order.price(), ask_order.volume()); 
            std::sprintf(asker_ptr->server_buffer,"%s:%s:%f:%f",bid_order.symbol().c_str(),bid_order.typeName().c_str(), bid_order.price(), bid_order.volume());

            //now send the two buffers and the client side is responsible for updating the portfolios

        }   
        */
        //go to the next pair
        it++;
    }
}

void Server::handle_client(Client* c){
    std::printf("Reached here, client hostname = %s\n", c->get_client_hostname_ptr());
    int n_bytes;
    int n;
    int msg_size = 0;
    //reset the buffer to 0 
    std::memset(c->client_buffer,0,c->buffer_size);
    OrderPacket *o;

    while(true){
        //infinite loop to handle receiving messages here

        //receive bytes from the client
        n_bytes = recv(c->active_socket_fd, c->client_buffer + msg_size, c->buffer_size,0);
        if(n_bytes <= 0){
            return;
        }
        msg_size += n_bytes;
        std::string buf_contents(c->client_buffer, c->client_buffer + msg_size);

        std::vector<std::string> tokens = generate_tokens(buf_contents);
        
        n = tokens.size();
        /*
        std::cout << "Number of tokens = " << n << std::endl;
        std::cout <<"Printing tokens:\n";
        for(int i=0;i<n;i++){
           std::cout << tokens[i] << " ";
        }
        std::cout << "\nDone printing tokens" << std::endl;
        */

        if(n > 0){
            //We got tokens, only need to keep the last one, get rid of the rest
            std::memset(c->client_buffer, 0, c->buffer_size);


            //If there is some stuff after the \r\n, then retain that, we need it. Don't use that last token
            if(!(msg_size > 2 && buf_contents[msg_size-2] == '\r' && buf_contents[msg_size-1] == '\n')){
                //std::printf("Oh no, last two characters are %d %d\n", buf_contents[msg_size-2], buf_contents[msg_size-1]);
                msg_size = tokens[n-1].size();
                std::memcpy(c->client_buffer, tokens[n-1].c_str(), tokens[n-1].size());
                n--;
            }else{
                msg_size = 0;   //adjust the contents of the client buffer accordingly
            }
        }else{
            //std::printf("Size of buffer = %d and contents of buffer are: %s\n", msg_size, c->client_buffer);
        }
        
        for(int i=0;i<n;i++){
            o = create_order_from_command(tokens[i]);
            this->create_order(o,c);
        }
    
    }
    
    return;
}