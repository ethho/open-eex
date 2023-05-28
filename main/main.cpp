#include <iostream>
#include <cstdio>
#include "server.hpp"

int main(int argc, char **argv){
    std::printf("Hi\n");
    Server s;
    s.create_server("6666");
}   