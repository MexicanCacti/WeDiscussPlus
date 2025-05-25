#pragma once

#include "MessageInterface.hpp"
#include "ConnectMessage.hpp"
#include "UserMessage.hpp"
#include "ChatroomMessage.hpp"
#include "AuthMessage.hpp"
#include "Chatroom.hpp"
#include "User.hpp"
#include <memory>

class MessageBuilder {
private:
    std::shared_ptr<MessageInterface> buildConnectMessage();

    std::shared_ptr<MessageInterface> buildAuthMessage();

    std::shared_ptr<MessageInterface> buildUserMessage();

    std::shared_ptr<MessageInterface> buildChatroomMessage();
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

public:
    MessageBuilder() : 
        _success_bit(true),
        _messageContents(""),
        _to_user_name(""),
        _to_user_id(-1),
        _from_user_name(""),
        _from_user_id(-1),
        _to_chatroom_id(-1),
        _from_chatroom_id(-1),
        _messageType(MessageType::UNDEFINED) {}

    void setSuccessBit(bool success);

    void setMessageContents(const std::string& contents);

    void setToUser(const std::string& username, int id);

    void setFromUser(const std::string& username, int id);

    void setToChatroom(int id);

    void setFromChatroom(int id);

    void setMessageType(MessageType type);

    std::shared_ptr<MessageInterface> build();
};