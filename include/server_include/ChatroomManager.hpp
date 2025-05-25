#pragma once

#include "Manager.hpp"
#include "Chatroom.hpp"
#include <chrono>
#include <shared_mutex>
using namespace std::chrono_literals;

class Server;

class ChatroomManager : public Manager {
    private:
        static std::unordered_map<int, Chatroom> _chatroomMap; // Cache of chatroomID : Chatroom object
        static std::shared_mutex _chatroomMapMutex; // Fine if multiple threads read, but only one thread writes.

        void createChatroom(std::shared_ptr<MessageInterface>& work);
        void deleteChatroom(std::shared_ptr<MessageInterface>& work);
        void addUserToChatroom(std::shared_ptr<MessageInterface>& work);
        void sendMessageToChatroom(std::shared_ptr<MessageInterface>& work);
        void removeUserFromChatroom(std::shared_ptr<MessageInterface>& work);
        void setUpDatabaseConnection() override;
        void processWork(std::shared_ptr<MessageInterface>& work) override;
    public:
        ChatroomManager(Server& server) : Manager(server) {this->setUpDatabaseConnection();}
};