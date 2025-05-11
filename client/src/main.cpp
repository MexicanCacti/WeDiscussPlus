#include <iostream>
#include "client_include/Client.hpp"
int main(){
    try {
        Client client("localhost", 3333);
        client.start();
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}