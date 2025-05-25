#pragma once

#include <string>
#include <vector>
#include <unordered_set>
#include "UserMessage.hpp"

class User{
    private:
        int _ID;
        std::string _username;
        std::string _password;
        bool _onlineStatus;
        bool _adminStatus;
        std::vector<std::shared_ptr<UserMessage>> _messageInbox;
        std::unordered_set<int> _involvedChatrooms;
    public:
        User() : _ID(0), _username(""), _password(""), _adminStatus(false), _onlineStatus(false) {};
        User(std::string& username, std::string& password, bool adminStatus, int ID): _username(username), _password(password), _adminStatus(adminStatus), _ID(ID) {};
        int getUserID() const {return _ID;}
        std::string getUsername() const {return _username;}
        std::string getPassword() const {return _password;}
        const std::vector<std::shared_ptr<UserMessage>>& getMessageInbox() const {return _messageInbox;}
        std::unordered_set<int> getInvolvedChatrooms() const {return _involvedChatrooms;}
        bool isInChatroom(int chatroomID) const {return _involvedChatrooms.find(chatroomID) != _involvedChatrooms.end();} 
        bool isOnline() const {return _onlineStatus;}
        bool isAdmin() const {return _adminStatus;}
        void addChatroom(const int chatroomID) {_involvedChatrooms.insert(chatroomID);}
        void setPassword(const std::string& password) {_password = password;}
        void addToInbox(const std::shared_ptr<UserMessage>& message) {_messageInbox.push_back(message);}
        void displayInbox();
};
