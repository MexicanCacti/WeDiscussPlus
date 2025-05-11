#ifndef CLIENTUI_HPP
#define CLIENTUI_HPP

#include <string>
#include <memory>
#include "Client.hpp"

class ClientUI {
    private:
        std::unique_ptr<Client> _client;
        bool _isAuthenticated;
        std::string _username;
        int _userId;

        // Private helper methods
        bool loginScreen();
        void mainMenu();
        void checkInbox();
        void getActiveUsers();
        void sendMessageToUser();
        void checkChatrooms();
        void displayMessage(const Message& message);

    public:
        ClientUI();
        void start();
};

#endif