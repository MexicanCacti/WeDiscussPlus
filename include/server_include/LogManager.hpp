#pragma once

#include "Manager.hpp"
#include "MessageInterface.hpp"
#include <memory>

class Server;

class LogManager : public Manager {
    private:
        void getToUserMessages(int userID);
        void getFromUserMessages(int userID);
        void getBetweenUsersMessages(int userID1, int userID2);
        void getChatroomMessages(int chatroomID);
        void getChatroomMessagesFromUser(int chatroomID, int userID);
        void answerLogRequest(std::shared_ptr<MessageInterface>& message);
        void processWork(std::shared_ptr<MessageInterface>& work) override;
        void setUpDatabaseConnection() override;

    public:
        void storeConnectLog(std::shared_ptr<MessageInterface>& message);
        LogManager(Server& server) : Manager(server) {this->setUpDatabaseConnection();}
};