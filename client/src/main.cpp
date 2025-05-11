#include <iostream>
#include "client_include/ClientUI.hpp"

int main() {
    try {
        ClientUI ui;
        ui.start();
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}