#include "client_include/ClientUI.hpp"
#include <iostream>
#include <limits>

ClientUI::ClientUI() : _isAuthenticated(false), _userId(-1) {}

void ClientUI::start() {
    if (!loginScreen()) {
        std::cout << "Login failed. Exiting..." << std::endl;
        return;
    }

    while (_isAuthenticated) {
        mainMenu();
    }
}

bool ClientUI::loginScreen() {
    std::string host;
    int port;
    std::string password;

    std::cout << "\n=== WeDiscuss++ Login ===\n";
    
    // Get server connection details
    std::cout << "Enter server IP: ";
    std::getline(std::cin, host);
    
    std::cout << "Enter server port: ";
    std::cin >> port;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    try {
        _client = std::make_unique<Client>(host, port);
    } catch (const std::exception& e) {
        std::cerr << "Failed to connect to server: " << e.what() << std::endl;
        return false;
    }

    // Get user credentials
    std::cout << "Enter username: ";
    std::getline(std::cin, _username);
    
    std::cout << "Enter password: ";
    std::getline(std::cin, password);

    // TODO: Implement actual authentication with server
    // For now, we'll just set authenticated to true
    _isAuthenticated = true;
    _userId = 1; // Temporary user ID

    std::cout << "Login successful!\n";
    return true;
}

void ClientUI::mainMenu() {
    std::cout << "\n=== Main Menu ===\n";
    std::cout << "1. Check Inbox\n";
    std::cout << "2. Get Active Users\n";
    std::cout << "3. Send Message To User\n";
    std::cout << "4. Check Chatrooms\n";
    std::cout << "5. Logout\n";
    std::cout << "Enter your choice (1-5): ";

    int choice;
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    switch (choice) {
        case 1:
            checkInbox();
            break;
        case 2:
            getActiveUsers();
            break;
        case 3:
            sendMessageToUser();
            break;
        case 4:
            checkChatrooms();
            break;
        case 5:
            _isAuthenticated = false;
            std::cout << "Logging out...\n";
            break;
        default:
            std::cout << "Invalid choice. Please try again.\n";
    }
}

void ClientUI::checkInbox() {
    std::cout << "\n=== Inbox ===\n";
    // TODO: Implement inbox checking with server
    std::cout << "Inbox functionality not yet implemented.\n";
}

void ClientUI::getActiveUsers() {
    std::cout << "\n=== Active Users ===\n";
    // TODO: Implement active users list with server
    std::cout << "Active users functionality not yet implemented.\n";
}

void ClientUI::sendMessageToUser() {
    std::cout << "\n=== Send Message ===\n";
    std::string recipient;
    std::string message;

    std::cout << "Enter recipient username: ";
    std::getline(std::cin, recipient);

    std::cout << "Enter your message: ";
    std::getline(std::cin, message);

    // TODO: Implement message sending with server
    std::cout << "Message sending functionality not yet implemented.\n";
}

void ClientUI::checkChatrooms() {
    std::cout << "\n=== Chatrooms ===\n";
    // TODO: Implement chatroom checking with server
    std::cout << "Chatroom functionality not yet implemented.\n";
}

void ClientUI::displayMessage(const Message& message) {
    std::cout << "From: " << message.getFromUserID() << "\n";
    std::cout << "To: " << message.getToUserID() << "\n";
    std::cout << "Message: " << message.getMessageContents() << "\n";
    std::cout << "------------------------\n";
}
