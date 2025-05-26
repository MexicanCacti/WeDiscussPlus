#pragma once

#include "MessageInterface.hpp"
#include "ConnectMessage.hpp"
#include "UserMessage.hpp"
#include "ChatroomMessage.hpp"
#include "AuthMessage.hpp"
#include "AuthResponseMessage.hpp"
#include "Chatroom.hpp"
#include "User.hpp"
#include <memory>

class MessageBuilder {
private:
    std::shared_ptr<MessageInterface> buildConnectMessage();

    std::shared_ptr<MessageInterface> buildAuthMessage();

    std::shared_ptr<MessageInterface> buildAuthResponseMessage();

    std::shared_ptr<MessageInterface> buildUserMessage();

    std::shared_ptr<MessageInterface> buildChatroomMessage();

    std::shared_ptr<MessageInterface> buildLogoutMessage();

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
    std::unordered_map<int, std::string> _userMap;
    std::vector<ChatroomData> _chatrooms;
    std::vector<std::shared_ptr<MessageInterface>> _inbox;

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

    void setUserMap(const std::unordered_map<int, std::string>& userMap);

    void setChatrooms(const std::vector<ChatroomData>& chatrooms);

    void setInbox(const std::vector<std::shared_ptr<MessageInterface>>& inbox);

    std::shared_ptr<MessageInterface> build();
};