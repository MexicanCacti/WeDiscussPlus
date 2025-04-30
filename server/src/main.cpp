#include <iostream>
#include <thread>
#include "server_include/Server.hpp"

void checkIsRunning(Server*);

int main(){
    Server server;

    std::thread serverMainThread(&Server::run, &server);

    std::thread checkServerStatus(checkIsRunning, &server);

    serverMainThread.join();
    checkServerStatus.join();

    std::cout << "Server Ended & Cleaned up\nPress any key to exit...";
    std::cin.get();
    return 0;
}

void checkIsRunning(Server* server){
    while(server->IsRunning()){
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
}