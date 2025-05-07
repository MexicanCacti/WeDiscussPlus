#ifndef USERMANAGER_HPP
#define USERMANAGER_HPP

#include <unordered_map>
#include <string>
#include <queue>
#include <atomic>
#include <mutex>
#include "server_include/Server.hpp"
#include "shared_include/User.hpp"

template<typename WorkType>
class UserManager{
    private:
        std::atomic<bool> _shouldDoWork = true;
        //std::weak_ptr<Server> _serverPointer = nullptr; //B/c weak_ptr ... remmeber to call lock() before to check if exists by getting shared_ptr

        // static std::unordered_map<int, User> _userMap;

        // static std::unordered_map<int, std::string> _userIDToName;

        // static std::unordered_map<std::string, int> _usernameToID;

        std::queue<WorkType> _processQueue;
        std::mutex _processQueueMutex;
    public:
        UserManager();
        void start();
        inline void stop() {_shouldDoWork = false;}
        void checkForWork();
        void addWork(WorkType& work);
        void authUser(WorkType* work);
        void logoutUser(WorkType* work);
        void addUser(WorkType* work);
        void changeUserPassword(WorkType* work);
        void changeUserName(WorkType* work);
        void deleteUser(WorkType* work);
        int getQueueSize() const {return _processQueue.size();}
        // SendMessage
        // Some Functions to get Users & info from database!
};

#endif