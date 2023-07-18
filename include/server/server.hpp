#pragma once
#include <string>
#include <cstring>
#include <cstdio>
#include <cerrno>
#include <thread>
#include <unordered_map>
//os related libraries
#include <unistd.h>

//not sure what the C++ equivalent headers are
#include <sys/socket.h> 
#include <sys/types.h>
#include <netdb.h>

#include "client.hpp"
#include "messages.hpp"
#include "matching_engine.hpp"
#include "scheduler.hpp"

class Server{
private:
    char *port;
    char *server_hostname;
    int backlog;
    int buffer_size; //use the same buffer size for all buffers
    int passive_socket_fd; //file descriptor for passive socket which listens for connections 
    std::unordered_map<std::string, MatchingEngine> m;
    RunTimeScheduler rs;
    std::vector<std::thread> all_threads;

    //This function takes an OrderPacket and puts an order into the queue
    void create_order(OrderPacket* o, Client* c); 

    void add_order(const Order& o);
    void add_bid(std::string ticker, double price_per_share, double volume, Client *c);
    void add_ask(std::string ticker, double price_per_share, double volume, Client *c);
    

    void run_matching_engine(); //this function is redundant
    void run_matching_engine(std::string ticker);
    void handle_client(Client* c);

public:
    Server();
    // This function sets up the socket stuff and listens for incoming connections
    void create_server(const char* port_p, int buffer_size = 1024, int backlog = 10); 
    
    // This function will end the server and close all connections
    void destroy_server(); 


    friend class Client;
};

void send_all(int sockfd, char* buf_ptr, int length, int flags);