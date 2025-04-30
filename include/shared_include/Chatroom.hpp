#include <unordered_map>
#include <map>
#include <string>
#include <shared_include/Message.hpp>
#ifndef CHATROOM_HPP
#define CHATROOM_HPP
class Chatroom{
    private:
        static int IDCounter;
        int ID;
        std::unordered_map<int, std::string> memberList; // ID : userName
        std::unordered_map<int,std::vector<Message>> userToMessage; // ID: Messages
        std::vector<Message> messageList; // Upon creation, messages corresponding to this chatroom should already be sorted by date in DB!
        const static int messageChunkDivisor = 4;
        int lastMessageSent = 0;
    public:
        Chatroom(int);
        inline int getChatroomID() const {return ID;}
        inline std::unordered_map<int, std::string> getMemberList() const {return memberList;}
        std::pair<int, std::string> findMember(int userID);
        inline void addMember(std::pair<int, std::string>& memberInfo) {memberList[memberInfo.first] = memberInfo.second;}
        inline void removeMember(int userID) {memberList.erase(userID);}
        std::pair<int,std::string> findMember(int);
        inline std::vector<Message> getAllMessages() const {return messageList;}
        std::vector<Message> getSomeMessages() const;
        void addMessage(Message);
        void displayMessages();
};


#endif