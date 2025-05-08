#ifndef USERMANAGER_HPP
#define USERMANAGER_HPP

#include "server_include/Manager.hpp"
#include "shared_include/Message.hpp"
#include "shared_include/User.hpp"

// Forward Declaration to avoid circular dependency
class Server;

template<typename WorkType>
class UserManager : public Manager<WorkType> {
    private:
        static std::unordered_map<int, User> _userMap; // Cache of userID : User object
        static std::unordered_map<int, std::string> _userIDToName; // Cache of userID : username
        static std::unordered_map<std::string, int> _usernameToID; // Cache of username : userID

        void authUser(WorkType* work);
        void logoutUser(WorkType* work);
        void addUser(WorkType* work);
        void changeUserPassword(WorkType* work);
        void changeUserName(WorkType* work);
        void deleteUser(WorkType* work);
        void sendMessageToUser(WorkType* work);

        void setUpDatabaseConnection() override;
    public:
        UserManager(Server& server) : Manager<WorkType>(server) {this->setUpDatabaseConnection();}
        void handleClient() override;
};

#endif