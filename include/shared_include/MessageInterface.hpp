#pragma once

#include "MessageType.hpp"
#include <string>
#include <vector>
#include <iostream>
#include <memory>

class MessageBuilder;
class MessageFactory;

class MessageInterface {
protected:
    bool _success_bit;
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

    friend class MessageBuilder; 
    friend class MessageFactory;
public:

    bool getSuccessBit() const { return _success_bit; }
    const std::string& getMessageContents() const { return _messageContents; }
    const std::string& getToUsername() const { return _to_user_name; }
    int getToUserID() const { return _to_user_id; }
    const std::string& getFromUsername() const { return _from_user_name; }
    int getFromUserID() const { return _from_user_id; }
    int getToChatroomID() const { return _to_chatroom_id; }
    int getFromChatroomID() const { return _from_chatroom_id; }
    MessageType getMessageType() const { return _messageType; }

    virtual std::vector<char> serialize() const = 0;
    virtual void deserialize(const std::vector<char>& data, size_t& offset) = 0;
    virtual void printMessage() const = 0;

    static std::string messageTypeToString(const MessageType& messageType);
    static std::shared_ptr<MessageInterface> createFromType(MessageType type);
};

