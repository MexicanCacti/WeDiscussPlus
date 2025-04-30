#include <unordered_map>
#include <string>
#include "shared_include/User.hpp"
#ifndef USERMANAGER_HPP
#define USERMANAGER_HPP
class UserManager{
    private:
        std::unordered_map<int, User> userMap;
        std::unordered_map<int, std::string> userIDToName;
        std::unordered_map<std::string, int> usernameToID;
    public:
        // Handle Client
        std::string getUsernameFor(int userID) const; 
        int getUserIDFor(const std::string& username) const;
        std::unordered_map<int, User> getUserMap() const {return userMap;}
        // Auth User
        // Add User
        // ChangeUserPassword
        // ChangeUserName
        // SendMessage
        // DeleteUser
        // Some Functions to get Users & info from database!
};

#endif