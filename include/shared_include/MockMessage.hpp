#ifndef MOCK_MESSAGE_HPP
#define MOCK_MESSAGE_HPP

#include "MessageInterface.hpp"
#include "MessageBuilder.hpp"
#include <iostream>

class MockMessage : public MessageInterface {
public:
    MockMessage() : MessageInterface() {};
    MockMessage(MessageBuilder<MockMessage>* messageBuilder);

    inline void setSuccessBit(bool success_bit) { _success_bit = success_bit; }
    inline void setMessageContents(const std::string& contents) { _messageContents = contents; }
    inline void setToUsername(const std::string& username) { _to_user_name = username; }
    inline void setToUserID(int id) { _to_user_id = id; }
    inline void setFromUsername(const std::string& username) { _from_user_name = username; }
    inline void setFromUserID(int id) { _from_user_id = id; }
    inline void setToChatroomID(int id) { _to_chatroom_id = id; }
    inline void setFromChatroomID(int id) { _from_chatroom_id = id; }
    inline void setMessageType(MessageType& type) { _messageType = type; }

    std::vector<char> serialize() const override;
    static MockMessage deserialize(const std::vector<char>& data);
    void printMessage() const override;
};

#endif 