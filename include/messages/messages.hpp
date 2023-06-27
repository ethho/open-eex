#pragma once
#include <vector>
#include <string>
#include <regex>
#include <iostream>

#include <cstdlib>
#include <cstring>

//os related libraries
#include <unistd.h>

//not sure what the C++ equivalent headers are
#include <sys/socket.h> 
#include <sys/types.h>
#include <netdb.h>

#include "scheduler.hpp"

//Forward declare the client class and server class
class Client;
class Server;


struct OrderPacket{
    /**
     * This is an intermediate struct which will be given to the scheduler.
     * The scheduler will then assign tasks to receive this struct and give it to the matching engine
    */
    char* ticker;
    bool true_if_bid;
    int num_shares;
    float price_per_share;
};

void tokenize(std::vector<std::string>& tokens, std::string command);

std::vector<std::string> generate_tokens(std::string command);

OrderPacket* create_order_from_command(std::string order_command);

void print_order_packet(OrderPacket* o);
