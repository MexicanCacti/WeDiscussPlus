#ifndef CHATROOMMANAGER_HPP
#define CHATROOMMANAGER_HPP

#include "Manager.hpp"
#include "Message.hpp"
#include "Chatroom.hpp"
#include "MockMessage.hpp"
#include <chrono>
#include <shared_mutex>
using namespace std::chrono_literals;

template<typename WorkType>
class Server;

template<typename WorkType>
class ChatroomManager : public Manager<WorkType> {
    private:
    static std::unordered_map<int, Chatroom> _chatroomMap; // Cache of chatroomID : Chatroom object
    static std::shared_mutex _chatroomMapMutex; // Fine if multiple threads read, but only one thread writes.

    void createChatroom(WorkType& work);
    void deleteChatroom(WorkType& work);
    void addUserToChatroom(WorkType& work);
    void sendMessageToChatroom(WorkType& work);
    void removeUserFromChatroom(WorkType& work);
    void setUpDatabaseConnection() override;
    void processWork(WorkType& work) override;
public:
    ChatroomManager(Server<WorkType>& server) : Manager<WorkType>(server) {this->setUpDatabaseConnection();}
};

#endif