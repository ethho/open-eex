#include "client.hpp"

void Client::create_client(Server *s,int sockfd, int buffer_size){
    this->server = s;
    this->active_socket_fd = sockfd;
    this->buffer_size = buffer_size;
    this->client_hostname = new char[this->buffer_size];
    this->client_buffer = new char[this->buffer_size];
    return;
}

char* Client::get_client_hostname_ptr(){
    return this->client_hostname;
}

char* Client:: get_client_buffer_ptr(){
    return this->client_buffer;
}

int Client::get_buffer_size(){
    return this->buffer_size;
}