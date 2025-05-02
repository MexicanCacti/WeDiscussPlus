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
        std::weak_ptr<Server> _serverPointer = nullptr; //B/c weak_ptr ... remmeber to call lock() before to check if exists by getting shared_ptr
        static bool setUpDone = false;

        static std::unordered_map<int, User> _userMap;
        std::mutex _userMapMutex;
        static std::unordered_map<int, std::string> _userIDToName;
        std::mutex _userIDToNameMutex;
        static std::unordered_map<std::string, int> _usernameToID;
        std::mutex _usernameToIDMutex;
        std::queue<WorkType> _processQueue; // ADD MUTEX CHECKING!
    public:
        inline UserManager(std::weak_ptr<Server> serverPointer) : _serverPointer(serverPointer) {;}
        void start();
        inline void stop() {_doWork = false;}
        void checkForWork();
        void addWork(WorkType& work) {_processQueue.push(work);}
        void authUser(WorkType* work);
        void logoutUser(WorkType* work);
        void addUser(WorkType* work);
        void changeUserPassword(WorkType* work);
        void changeUserName(WorkType* work);
        void deleteUser(WorkType* work);
        // SendMessage
        // Some Functions to get Users & info from database!
};

#endif