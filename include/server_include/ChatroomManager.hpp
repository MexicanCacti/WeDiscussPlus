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

    void createChatroom(std::pair<WorkType, std::shared_ptr<tcp::socket>>& work);
    void deleteChatroom(std::pair<WorkType, std::shared_ptr<tcp::socket>>& work);
    void addUserToChatroom(std::pair<WorkType, std::shared_ptr<tcp::socket>>& work);
    void sendMessageToChatroom(std::pair<WorkType, std::shared_ptr<tcp::socket>>& work);
    void removeUserFromChatroom(std::pair<WorkType, std::shared_ptr<tcp::socket>>& work);
    void setUpDatabaseConnection() override;
    void processWork(std::pair<WorkType, std::shared_ptr<tcp::socket>>& work) override;
public:
    ChatroomManager(Server& server) : Manager<WorkType>(server) {this->setUpDatabaseConnection();}
};

#endif