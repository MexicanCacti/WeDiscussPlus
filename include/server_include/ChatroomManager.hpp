#ifndef CHATROOMMANAGER_HPP
#define CHATROOMMANAGER_HPP

#include <unordered_map>
#include <queue>
#include <atomic>
#include <mutex>
#include "server_include/Server.hpp"
#include "shared_include/Chatroom.hpp"
template<typename WorkType>
class ChatroomManager{
    private:
        std::atomic<bool> _shouldDoWork = true;
        //std::weak_ptr<Server> _serverPointer = nullptr; //B/c weak_ptr ... remmeber to call lock() before to check if exists by getting shared_ptr
        // chatroom Map ... Integer : Chatroom ... Again just use the DB to search/insert chatroom info!

        std::queue<WorkType> _processQueue;
        std::mutex _processQueueMutex;
public:
    ChatroomManager();
    void start();
    inline void stop() {_shouldDoWork = false;}
    void checkForWork();
    void addWork(WorkType& work);
    void addUserToChatroom(WorkType* work);
    void sendMessageToChatroom(WorkType* work);
    void createChatroom(WorkType* work);
    void deleteChatroom(WorkType* work);
    void removeUserFromChatroom(WorkType* work);
    int getQueueSize() const {return _processQueue.size();}
};

#endif