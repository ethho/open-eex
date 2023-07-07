#include "messages.hpp"

//Use delimiter ;; so that users do not get confused
const char* delim = "\r\n";

void tokenize(std::vector<std::string>& tokens, std::string command){
    std::regex re(delim);

    //i don't like iterators
    std::sregex_token_iterator start(command.begin(),command.end(), re,-1);
    std::sregex_token_iterator end = std::sregex_token_iterator();
    //copy the elements of the regex iterator into the vector
    std::sregex_token_iterator it;
    for(it = start; it != end; it++){
        tokens.push_back(it->str());
    }

    // Additional check to remove empty strings
    tokens.erase(std::remove_if(tokens.begin(),tokens.end(), [](std::string const& s) {return s.size() == 0;}),tokens.end());
    return;
}

void tokenize(std::vector<std::string>& tokens, char* command){
    char *token;
    char *rest = command;
 
    while ((token = strtok_r(rest, delim, &rest))){
        tokens.push_back(std::string(token));
    }
    return;
}

std::vector<std::string> generate_tokens(std::string command){
    std::vector<std::string> tokens;

    //if the delimiter is not present, then do not do anything
    
    if(command.find(delim) == std::string::npos){
        std::cout <<"Didn't find\n";
        return tokens;
    }
    
    //otherwise, you're free to tokenize
    if(command.size() > 0){
        tokenize(tokens,command.c_str());
    }

    return tokens;
}

OrderPacket* create_order_from_command(std::string order_command){
    std::vector<std::string> tokens;

    //Use delimiter : 
    std::regex re(":");

    //i don't like iterators
    std::sregex_token_iterator start(order_command.begin(),order_command.end(), re,-1);
    std::sregex_token_iterator end = std::sregex_token_iterator();
    //copy the elements of the regex iterator into the vector
    std::sregex_token_iterator it;
    for(it = start; it != end; it++){
        tokens.push_back(it->str());
    }
    //std::printf("%s %s %s %s", tokens[0], tokens[1], tokens[2], tokens[3]);

    if(tokens.size() != 4){
        std::printf("Wrong format of message sent");
        return nullptr;
    }

    OrderPacket *new_packet = new OrderPacket();

    //Allocate new memory for this because tokens[0] will get deleted after it exits the scope of the function
    new_packet->ticker = new char[tokens[0].size()];
    std::strcpy(new_packet->ticker,tokens[0].c_str());
    
    new_packet->true_if_bid = tokens[1] == std::string("BID");
    new_packet->price_per_share = std::stod(tokens[2]);
    new_packet->num_shares = std::stod(tokens[3]);
    print_order_packet(new_packet);
    return new_packet;
}

void print_order_packet(OrderPacket* o){
    std::cout << "ORDER = " << o->ticker << " " << o->true_if_bid << " " << o->price_per_share << " " << o->num_shares << "\n";
    return;
}


void eex_log(char *fmt, ...)
{
    va_list argptr;

    va_start(argptr, fmt);

    flockfile(stdout);
    std::vprintf(fmt, argptr);
    funlockfile(stdout);
    fflush(stdout);

    va_end(argptr);
}

void eex_log(const char *fmt, ...)
{
    va_list argptr;

    va_start(argptr, fmt);

    flockfile(stdout);
    std::vprintf(fmt, argptr);
    funlockfile(stdout);
    fflush(stdout);

    va_end(argptr);
}