#pragma once
#include <string>
#include <cstring>
#include <cstdio>
#include <cerrno>
#include <thread>

//not part of the standard so they never made cunistd.h.... So weird
#include <unistd.h>

//not sure what the C++ equivalent headers are
#include <sys/socket.h> 
#include <sys/types.h>
#include <netdb.h>

#include "client.hpp"


class Server{
private:
    char *port;
    char *server_hostname;
    int backlog;
    int buffer_size; //use the same buffer size for all buffers
    int passive_socket_fd; //file descriptor for passive socket which listens for connections 

public:
    // This function sets up the socket stuff and listens for incoming connections
    void create_server(char* port_p, int buffer_size = 1024, int backlog = 10); 
    
    // This function will end the server and close all connections
    void destroy_server(); 
};

void handle_client(Client *c);