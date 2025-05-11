#ifndef MOCK_MESSAGE_HPP
#define MOCK_MESSAGE_HPP

#include "MessageType.hpp"
#include "MessageBuilder.hpp"
#include <string>
#include <vector>
#include <iostream>

class MockMessage {
private:
    std::string _messageContents;
    std::string _to_user_name;
    int _to_user_id;
    std::string _from_user_name;
    int _from_user_id;
    int _to_chatroom_id;
    int _from_chatroom_id;
    MessageType _messageType;
    static void serializeInt(std::vector<char>& buffer, int val);
    static int deserializeInt(const std::vector<char>& buffer, size_t& offset);
    static void serializeString(std::vector<char>& buffer, const std::string& str);
    static std::string deserializeString(const std::vector<char>& buffer, size_t& offset);
public:
    MockMessage() : _messageContents(""), _to_user_name(""), _to_user_id(-1), _from_user_name(""), _from_user_id(-1), _to_chatroom_id(-1), _from_chatroom_id(-1), _messageType(MessageType::UNDEFINED) {}
    MockMessage(MessageBuilder<MockMessage>* builder);

    std::string getMessageContents() const { return _messageContents; }
    std::string getToUsername() const { return _to_user_name; }
    int getToUserID() const { return _to_user_id; }
    std::string getFromUsername() const { return _from_user_name; }
    int getFromUserID() const { return _from_user_id; }
    int getToChatroomID() const { return _to_chatroom_id; }
    int getFromChatroomID() const { return _from_chatroom_id; }
    MessageType getMessageType() const { return _messageType; }

    void setMessageContents(const std::string& contents) { _messageContents = contents; }
    void setToUsername(const std::string& username) { _to_user_name = username; }
    void setToUserID(int id) { _to_user_id = id; }
    void setFromUsername(const std::string& username) { _from_user_name = username; }
    void setFromUserID(int id) { _from_user_id = id; }
    void setToChatroomID(int id) { _to_chatroom_id = id; }
    void setFromChatroomID(int id) { _from_chatroom_id = id; }
    void setMessageType(MessageType& type) { _messageType = type; }

    std::vector<char> serialize() const;
    void printMessage() const;

    static MockMessage deserialize(const std::vector<char>& data);
    static std::string messageTypeToString(const MessageType& messageType);
};

#endif 