#include "client.hpp"

void Client::create_client(Server *s,int sockfd, int buffer_size){
    this->server = s;
    this->active_socket_fd = sockfd;
    this->buffer_size = buffer_size;
    this->client_hostname = new char[this->buffer_size];
    return;
}

char* Client::get_client_hostname_ptr(){
    return this->client_hostname;
}