#include <iostream>
#include <string>
#include <thread>
#include <atomic>
#include "Server.hpp"

static const int defaultPort = 3333;
static const std::string defaultIP = "127.0.0.1";

int main(int argc, char* argv[]) {
    #ifdef _MOCK_TESTING
        std::cout << "Mock testing enabled" << std::endl;
    #endif
    int port = defaultPort;
    std::string ip = defaultIP;

    #ifndef NDEBUG
        std::cout << "Enter IP address to bind to | Default:[" << defaultIP << "]: ";
        std::string inputIP;
        std::getline(std::cin, inputIP);
        if (!inputIP.empty()) {
            ip = inputIP;
        }

        std::cout << "Enter Port Number to Host on | Default: [" << defaultPort << "]: ";
        std::string inputPort;
        std::getline(std::cin, inputPort);
        if (!inputPort.empty()) {
            try {
                port = std::stoi(inputPort);
                if (port <= 0 || port > 65535) {
                    std::cerr << "Invalid port number! Using default port: " << defaultPort << std::endl;
                    port = defaultPort;
                }
            } catch (const std::exception& e) {
                std::cerr << "Invalid input! Using default port: " << defaultPort << std::endl;
                std::cerr << "Error: " << e.what() << std::endl;
                port = defaultPort;
            }
        }
    #endif

    std::cout << "Starting server on " << ip << ":" << port << "..." << std::endl;
    auto server = Server(ip, port);
    
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