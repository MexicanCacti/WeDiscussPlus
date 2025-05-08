#ifndef CHATROOMMANAGER_HPP
#define CHATROOMMANAGER_HPP


#include "server_include/Manager.hpp"
#include "shared_include/Message.hpp"
#include "shared_include/Chatroom.hpp"

class Server;

template<typename WorkType>
class ChatroomManager : public Manager<WorkType>{
    private:
    static std::unordered_map<int, Chatroom> _chatroomMap; // Cache of chatroomID : Chatroom object
    std::mutex _chatroomMapMutex;

    void addUserToChatroom(WorkType* work);
    void sendMessageToChatroom(WorkType* work);
    void createChatroom(WorkType* work);
    void deleteChatroom(WorkType* work);
    void removeUserFromChatroom(WorkType* work);
    void setUpDatabaseConnection() override;
public:
    ChatroomManager(Server& server) : Manager<WorkType>(server) {this->setUpDatabaseConnection();}
    void handleClient() override;
};

#endif