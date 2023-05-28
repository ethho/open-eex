#pragma once
#include <cstring>
#include <cstdio>
//forward declare the server class
class Server;

class Client{
private:
    char *client_hostname;
    int buffer_size;
    int active_socket_fd;
    Server *server;

public:
    void create_client(Server *s,int sockfd, int buffer_size);
    char* get_client_hostname_ptr();

    friend class Server;
};