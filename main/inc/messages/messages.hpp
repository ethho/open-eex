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

//Forward declare the client class and server class
class Client;
class Server;

void tokenize(std::vector<std::string>& tokens, std::string command);

std::vector<std::string> generate_tokens(std::string command);

