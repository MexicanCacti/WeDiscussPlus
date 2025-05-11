#ifndef CHATROOMMANAGER_HPP
#define CHATROOMMANAGER_HPP

#include "Manager.hpp"
#include "Message.hpp"
#include "Chatroom.hpp"
#include "MockMessage.hpp"

template<typename WorkType>
class Server;

template<typename WorkType>
class ChatroomManager : public Manager<WorkType> {
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
    ChatroomManager(Server<WorkType>& server) : Manager<WorkType>(server) {this->setUpDatabaseConnection();}
};

#endif