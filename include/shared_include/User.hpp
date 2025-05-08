#ifndef USER_HPP
#define USER_HPP

#include <string>
#include <vector>
#include <unordered_set>
#include "shared_include/Message.hpp"

class User{
    private:
        static int _userIDCounter;
        int _ID;
        std::string _username;
        std::string _password;
        bool _onlineStatus;
        bool _adminStatus;
        std::vector<Message> _messageInbox;
        std::unordered_set<int> _involvedChatrooms;
    public:
        inline User(std::string& username, std::string& password, bool adminStatus, int ID): _username(username), _password(password), _adminStatus(adminStatus), _ID(ID) {};
        inline int getUserID() const {return _ID;}
        inline std::string getUsername() const {return _username;}
        inline std::string getPassword() const {return _password;}
        inline std::vector<Message> getMessageInbox() const {return _messageInbox;}
        inline std::unordered_set<int> getInvolvedChatrooms() const {return _involvedChatrooms;}
        inline bool isInChatroom(int chatroomID) const {return _involvedChatrooms.find(chatroomID) != _involvedChatrooms.end();} 
        inline bool isOnline() const {return _onlineStatus;}
        inline bool isAdmin() const {return _adminStatus;}
        inline void addChatroom(const int chatroomID) {_involvedChatrooms.insert(chatroomID);}
        inline void setPassword(const std::string& password) {_password = password;}
        void addToInbox(const Message& message) {_messageInbox.push_back(message);}
        void displayInbox();
};
#endif