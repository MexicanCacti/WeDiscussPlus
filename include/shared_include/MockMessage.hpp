#ifndef MOCK_MESSAGE_HPP
#define MOCK_MESSAGE_HPP

#include "MessageInterface.hpp"
#include "MessageBuilder.hpp"
#include <iostream>

class MockMessage : public MessageInterface {
public:
    MockMessage() : MessageInterface() {};
    MockMessage(MessageBuilder<MockMessage>* messageBuilder);

    void setMessageContents(const std::string& contents) { _messageContents = contents; }
    void setToUsername(const std::string& username) { _to_user_name = username; }
    void setToUserID(int id) { _to_user_id = id; }
    void setFromUsername(const std::string& username) { _from_user_name = username; }
    void setFromUserID(int id) { _from_user_id = id; }
    void setToChatroomID(int id) { _to_chatroom_id = id; }
    void setFromChatroomID(int id) { _from_chatroom_id = id; }
    void setMessageType(MessageType& type) { _messageType = type; }

    std::vector<char> serialize() const override;
    static MockMessage deserialize(const std::vector<char>& data);
    void printMessage() const override;
};

#endif 