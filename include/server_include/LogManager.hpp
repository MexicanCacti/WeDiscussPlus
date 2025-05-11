#ifndef LOGMANAGER_HPP
#define LOGMANAGER_HPP

#include "server_include/Server.hpp"
#include "shared_include/MessageInterface.hpp"
#include <asio.hpp>
#include <memory>

using asio::ip::tcp;

class Server;

class LogManager {
    private:
        void getUserMessages(int userID);
        void getChatroomMessages(int chatroomID);
        void answerLogRequest(std::pair<MessageInterface, std::shared_ptr<tcp::socket>>& message);

    public:
        void setUpDatabaseConnection();
        void processWork(std::pair<MessageInterface, std::shared_ptr<tcp::socket>>& work);
        void storeMessage(MessageInterface& message);
        LogManager(Server& server) {this->setUpDatabaseConnection();}
};

#endif