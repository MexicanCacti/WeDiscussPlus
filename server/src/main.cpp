#include <iostream>
#include <string>
#include <thread>
#include <atomic>
#include <filesystem>
#include "Server.hpp"

static const int defaultPort = 3333;
static const std::string defaultIP = "127.0.0.1";

// Get the executable's directory
std::filesystem::path getExecutableDir() {
    #ifdef _WIN32
        char path[MAX_PATH];
        GetModuleFileNameA(NULL, path, MAX_PATH);
        return std::filesystem::path(path).parent_path();
    #else
        return std::filesystem::current_path();
    #endif
}

// Get database paths relative to executable
std::pair<std::string, std::string> getDatabasePaths() {
    std::filesystem::path exeDir = getExecutableDir();
    std::string dbPath = (exeDir / "server_database" / "database.db").string();
    std::string schemaPath = (exeDir / "server_database" / "schema.sql").string();
    return {dbPath, schemaPath};
}

int main(int argc, char* argv[]) {
    #ifdef _MOCK_TESTING
        std::cout << "Mock testing enabled" << std::endl;
    #endif

    auto [dbPath, schemaPath] = getDatabasePaths();
    std::cout << "Database path: " << dbPath << std::endl;
    std::cout << "Schema path: " << schemaPath << std::endl;

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
    auto server = Server(ip, port, dbPath, schemaPath);
    
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