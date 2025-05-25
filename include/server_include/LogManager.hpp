#pragma once

#include "Manager.hpp"
#include "MessageInterface.hpp"
#include <memory>

class Server;

class LogManager : public Manager {
    private:
        void getUserMessages(int userID);
        void getChatroomMessages(int chatroomID);
        void answerLogRequest(std::shared_ptr<MessageInterface>& message);
        void processWork(std::shared_ptr<MessageInterface>& work) override;
        void setUpDatabaseConnection() override;

    public:
        void storeMessage(std::shared_ptr<MessageInterface>& message);
        LogManager(Server& server) : Manager(server) {this->setUpDatabaseConnection();}
};