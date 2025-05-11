#include <iostream>
#include <string>
#include <thread>
#include <atomic>
#include "Message.hpp"
#include "Server.hpp"

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
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    #endif

    std::cout << "Starting server on port " << port << "..." << std::endl;
    auto server = Server<Message>(port);
    
    std::thread serverThread([&server]() {
        try {
            server.startServer();
        } catch (const std::exception& e) {
            std::cerr << "Server error: " << e.what() << std::endl;
        }
    });

    std::cout << "Server is running. Press Enter to stop..." << std::endl;
    std::cin.get();

    std::cout << "Stopping server..." << std::endl;
    server.stopServer();
    

    if (serverThread.joinable()) {
        serverThread.join();
    }

    std::cout << "Server stopped successfully." << std::endl;
    return 0;
}