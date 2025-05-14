#ifndef USERMANAGER_HPP
#define USERMANAGER_HPP

#include <unordered_map>
#include <string>
#include <memory>
#include <mutex>
#include <shared_mutex>
#include "Manager.hpp"
#include "User.hpp"

// Forward Declaration to avoid circular dependency
template<typename WorkType>
class Server;

template<typename WorkType>
class UserManager : public Manager<WorkType> {
    private:
        static std::unordered_map<int, User> _userMap;
        static std::unordered_map<std::string, int> _usernameToUserID;
        static std::unordered_map<int, std::string> _userIDToUsername;
        static std::shared_mutex _userMapMutex;
        static std::shared_mutex _userLookupMutex;

        // Track which client handlers have been started
        static std::unordered_map<int, bool> _clientHandlerStarted;

        /*
        std::optional<std::mutex&> getUserMutex(int userID);
        bool addUserMutex(int userID);
        bool removeUserMutex(int userID);
        inline bool hasUserMutex(int userID) { return _userMap.find(userID) != _userMap.end(); }
        */

        void authUser(WorkType& work);
        void logoutUser(WorkType& work);
        void addUser(WorkType& work);
        void changeUserPassword(WorkType& work);
        void changeUserName(WorkType& work);
        void deleteUser(WorkType& work);
        void sendMessageToUser(WorkType& work);
        void processWork(WorkType& work) override;
        void setUpDatabaseConnection() override;
    public:
        UserManager(Server<WorkType>& server) : Manager<WorkType>(server) {this->setUpDatabaseConnection();}
};

// Initialize static members
template<typename WorkType>
std::unordered_map<int, User> UserManager<WorkType>::_userMap;

template<typename WorkType>
std::unordered_map<std::string, int> UserManager<WorkType>::_usernameToUserID;

template<typename WorkType>
std::unordered_map<int, std::string> UserManager<WorkType>::_userIDToUsername;

template<typename WorkType>
std::shared_mutex UserManager<WorkType>::_userMapMutex;

template<typename WorkType>
std::shared_mutex UserManager<WorkType>::_userLookupMutex;

template<typename WorkType>
std::unordered_map<int, bool> UserManager<WorkType>::_clientHandlerStarted;

#endif