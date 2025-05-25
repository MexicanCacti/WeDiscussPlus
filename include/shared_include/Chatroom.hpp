#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include "ChatroomMessage.hpp"

class Chatroom {
private:
    int _ID;
    std::unordered_map<int, std::string> _memberList; // ID : userName
    std::unordered_map<int, std::vector<std::shared_ptr<ChatroomMessage>>> _userToMessage; // ID: Messages
    std::vector<std::shared_ptr<ChatroomMessage>> _messageList; // Message List should be sorted by date sent, newest last
    const static int _messageChunkDivisor = 4; // This is the number of messages to send to a client at a time. Clients can request older messages
    std::unordered_map<int, int> _userToLastMessageSent; // ID: # of last message sent

public:
    Chatroom(int);
    inline int getChatroomID() const { return _ID; }
    inline const std::unordered_map<int, std::string>& getMemberList() const { return _memberList; }
    inline void addMember(std::pair<int, std::string>& memberInfo) { _memberList[memberInfo.first] = memberInfo.second; }
    inline void removeMember(int userID) { _memberList.erase(userID); }
    std::pair<int,std::string> findMember(int);
    inline const std::vector<std::shared_ptr<ChatroomMessage>>& getAllMessages() const { return _messageList; }
    std::vector<std::shared_ptr<ChatroomMessage>> getSomeMessages() const;
    void addMessage(const std::shared_ptr<ChatroomMessage>& message);
    void displayMessages();
    std::vector<int> getUsers() const;
};