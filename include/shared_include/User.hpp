#include <string>
#include <vector>
#include <unordered_set>
#include "shared_include/Message.hpp"
#ifndef USER_HPP
#define USER_HPP
class User{
    private:
        static int userIDCounter;
        std::string username;
        std::string password;
        int ID;
        bool onlineStatus;
        bool adminStatus;
        std::vector<Message> messageInbox;
        std::unordered_set<int> involvedChatrooms;
    public:
        inline User(std::string& username, std::string& password, bool adminStatus, int ID): username(username), password(password), adminStatus(adminStatus), ID(ID) {};
        inline int getUserID() const {return ID;}
        inline std::string getUsername() const {return username;}
        inline std::string getPassword() const {return password;}
        inline std::vector<Message> getMessageInbox() const {return messageInbox;}
        inline std::unordered_set<int> getInvolvedChatrooms() const {return involvedChatrooms;}
        inline bool isInChatroom(int chatroomID) const {return involvedChatrooms.find(chatroomID) != involvedChatrooms.end();} 
        inline bool isOnline() const {return onlineStatus;}
        inline bool isAdmin() const {return adminStatus;}
        inline void addChatroom(const int chatroomID) {involvedChatrooms.insert(chatroomID);}
        inline void setPassword(const std::string& password) {this->password = password;}
        void addToInbox(const Message& message) {messageInbox.push_back(message);}
        void displayInbox();
};
#endif