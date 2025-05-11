#ifndef USERMANAGER_HPP
#define USERMANAGER_HPP

#include "Manager.hpp"
#include "Message.hpp"
#include "User.hpp"

// Forward Declaration to avoid circular dependency
template<typename WorkType>
class Server;

template<typename WorkType>
class UserManager : public Manager<WorkType> {
    private:
        static std::unordered_map<int, User> _userMap; // Cache of userID : User object
        static std::unordered_map<int, std::string> _userIDToName; // Cache of userID : username
        static std::unordered_map<std::string, int> _usernameToID; // Cache of username : userID

        void authUser(std::pair<WorkType, std::shared_ptr<tcp::socket>>&work);
        void logoutUser(std::pair<WorkType, std::shared_ptr<tcp::socket>>& work);
        void addUser(std::pair<WorkType, std::shared_ptr<tcp::socket>>& work);
        void changeUserPassword(std::pair<WorkType, std::shared_ptr<tcp::socket>>& work);
        void changeUserName(std::pair<WorkType, std::shared_ptr<tcp::socket>>& work);
        void deleteUser(std::pair<WorkType, std::shared_ptr<tcp::socket>>& work);
        void sendMessageToUser(std::pair<WorkType, std::shared_ptr<tcp::socket>>& work);
        void processWork(std::pair<WorkType, std::shared_ptr<tcp::socket>>& work) override;
        void setUpDatabaseConnection() override;
    public:
        UserManager(Server<WorkType>& server) : Manager<WorkType>(server) {this->setUpDatabaseConnection();}
};

#endif