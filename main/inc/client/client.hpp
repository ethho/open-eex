#pragma once
#include <cstring>
#include <cstdio>

//forward declare the server class
class Server;

class Client{
public:
    char* client_hostname;
    char* client_buffer;
    int buffer_size;
    int active_socket_fd;
    Server *server;
    void create_client(Server *s,int sockfd, int buffer_size);
    char* get_client_hostname_ptr();
    char* get_client_buffer_ptr();
    int get_buffer_size();
    friend class Server;
};