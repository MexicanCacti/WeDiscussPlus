#pragma once

#include <unordered_map>
#include <string>
#include <memory>
#include <mutex>
#include <shared_mutex>
#include "Manager.hpp"
#include "User.hpp"


class Server;

class UserManager : public Manager {
    private:
        static std::unordered_map<std::string, int> _usernameToUserID;
        static std::unordered_map<int, std::string> _userIDToUsername;
        static std::shared_mutex _userLookupMutex;

        // Track which client handlers have been started
        static std::unordered_map<int, bool> _clientHandlerStarted;

        void authUser(std::shared_ptr<MessageInterface>& work);
        void logoutUser(std::shared_ptr<MessageInterface>& work);
        void addUser(std::shared_ptr<MessageInterface>& work);
        void changeUserPassword(std::shared_ptr<MessageInterface>& work);
        void changeUserName(std::shared_ptr<MessageInterface>& work);
        void deleteUser(std::shared_ptr<MessageInterface>& work);
        void sendMessageToUser(std::shared_ptr<MessageInterface>& work);
        void processWork(std::shared_ptr<MessageInterface>& work) override;
        void setUpDatabaseConnection() override;
    public:
        UserManager(Server& server) : Manager(server) {this->setUpDatabaseConnection();}
};