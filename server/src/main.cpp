#include <iostream>
#include <string>
#include "server_include/Server.hpp"

static const int defaultPort = 13400;

int main(int argc, char* argv[]){
    int port = defaultPort;
    #ifndef NDEBUG
        std::cout << "Enter Port Number to Host on: ";
        std::cin >> port;
        if(std::cin.fail() || port <= 0 || port > 65535){
            std::cerr << "Invalid Input! Using Default Port: " << defaultPort << std::endl;
            port = defaultPort;
        }
    #endif

    auto server = Server(port);
    // Eventually, we should have server run on its own thread, then here the user can input commands to stop the server.
    server.startServer();
    std::cout << "Server Ended & Cleaned up\nPress any key to exit...";
    std::cin.get();
    return 0;
}