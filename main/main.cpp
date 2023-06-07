#include <iostream>
#include <cstdio>
#include "server.hpp"

int main(int argc, char **argv){
    std::printf("Hi\n");
    Server s;
    const char* port = "6661"; 
    s.create_server(port);
}   