#ifndef CHATROOM_HPP
#define CHATROOM_HPP

#include <unordered_map>
#include <string>
#include <vector>
#include "Message.hpp"

class Chatroom{
    private:
        static int _IDCounter;
        int _ID;
        std::unordered_map<int, std::string> _memberList; // ID : userName
        std::unordered_map<int,std::vector<Message>> _userToMessage; // ID: Messages
        std::vector<Message> _messageList; // Upon creation, messages corresponding to this chatroom should already be sorted by date in DB!
        const static int _messageChunkDivisor = 4;
        int _lastMessageSent = 0;
    public:
        Chatroom(int);
        inline int getChatroomID() const {return _ID;}
        inline std::unordered_map<int, std::string> getMemberList() const {return _memberList;}
        inline void addMember(std::pair<int, std::string>& memberInfo) {_memberList[memberInfo.first] = memberInfo.second;}
        inline void removeMember(int userID) {_memberList.erase(userID);}
        std::pair<int,std::string> findMember(int);
        inline std::vector<Message> getAllMessages() const {return _messageList;}
        std::vector<Message> getSomeMessages() const;
        void addMessage(Message);
        void displayMessages();
};


#endif