#ifndef MOCK_MESSAGE_HPP
#define MOCK_MESSAGE_HPP

#include "shared_include/MessageInterface.hpp"
#include "shared_include/MessageBuilder.hpp"
#include <string>

class MockMessage : public MessageInterface {
private:
    std::string _messageContents;
    std::string _to_user_name;
    int _to_user_id;
    std::string _from_user_name;
    int _from_user_id;
    int _to_chatroom_id;
    int _from_chatroom_id;
    MessageType _messageType;

public:
    MockMessage() : _messageContents(""), _to_user_name(""), _to_user_id(-1), _from_user_name(""), _from_user_id(-1), _to_chatroom_id(-1), _from_chatroom_id(-1), _messageType(MessageType::UNDEFINED) {}
    MockMessage(MessageBuilder builder) {
        _messageContents = builder.getMessageContents();
        _to_user_name = builder.getToUsername();
        _to_user_id = builder.getToUserID();
        _from_user_name = builder.getFromUsername();
        _from_user_id = builder.getFromUserID();
        _to_chatroom_id = builder.getToChatroomID();
        _from_chatroom_id = builder.getFromChatroomID();
        _messageType = builder.getMessageType();
    }

    std::string getMessageContents() const override { return _messageContents; }
    std::string getToUsername() const override { return _to_user_name; }
    int getToUserID() const override { return _to_user_id; }
    std::string getFromUsername() const override { return _from_user_name; }
    int getFromUserID() const override { return _from_user_id; }
    int getToChatroomID() const override { return _to_chatroom_id; }
    int getFromChatroomID() const override { return _from_chatroom_id; }
    MessageType getMessageType() const override { return _messageType; }

    void setMessageContents(const std::string& contents) override { _messageContents = contents; }
    void setToUsername(const std::string& username) override { _to_user_name = username; }
    void setToUserID(int id) override { _to_user_id = id; }
    void setFromUsername(const std::string& username) override { _from_user_name = username; }
    void setFromUserID(int id) override { _from_user_id = id; }
    void setToChatroomID(int id) override { _to_chatroom_id = id; }
    void setFromChatroomID(int id) override { _from_chatroom_id = id; }
    void setMessageType(MessageType type) override { _messageType = type; }

    std::vector<char> serialize() const override;
    void printMessage() const override;

    static MockMessage deserialize(const std::vector<char>& data);
};

#endif 