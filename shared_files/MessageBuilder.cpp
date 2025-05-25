#include "MessageBuilder.hpp"
#include "MessageInterface.hpp"
#include "ConnectMessage.hpp"
#include "AuthMessage.hpp"
#include "UserMessage.hpp"
#include "ChatroomMessage.hpp"

void MessageBuilder::setSuccessBit(bool success) {
    _success_bit = success;
}

void MessageBuilder::setMessageContents(const std::string& contents) {
    _messageContents = contents;
}

void MessageBuilder::setToUser(const std::string& username, int id) {
    _to_user_name = username;
    _to_user_id = id;
}

void MessageBuilder::setFromUser(const std::string& username, int id) {
    _from_user_name = username;
    _from_user_id = id;
}

void MessageBuilder::setToChatroom(int id) {
    _to_chatroom_id = id;
}

void MessageBuilder::setFromChatroom(int id) {
    _from_chatroom_id = id;
}

void MessageBuilder::setMessageType(MessageType type) {
    _messageType = type;
}

std::shared_ptr<MessageInterface> MessageBuilder::buildConnectMessage() {
    auto msg = std::make_shared<ConnectMessage>();
    msg->_success_bit = _success_bit;
    msg->_messageContents = _messageContents;
    msg->_to_user_name = _to_user_name;
    msg->_to_user_id = _to_user_id;
    msg->_from_user_name = _from_user_name;
    msg->_from_user_id = _from_user_id;
    return msg;
}

std::shared_ptr<MessageInterface> MessageBuilder::buildAuthMessage() {
    auto msg = std::make_shared<AuthMessage>();
    msg->_success_bit = _success_bit;
    msg->_messageContents = _messageContents;
    msg->_to_user_name = _to_user_name;
    msg->_to_user_id = _to_user_id;
    msg->_from_user_name = _from_user_name;
    msg->_from_user_id = _from_user_id;
    return msg;
}

std::shared_ptr<MessageInterface> MessageBuilder::buildUserMessage() {
    auto msg = std::make_shared<UserMessage>();
    msg->_success_bit = _success_bit;
    msg->_messageContents = _messageContents;
    msg->_to_user_name = _to_user_name;
    msg->_to_user_id = _to_user_id;
    msg->_from_user_name = _from_user_name;
    msg->_from_user_id = _from_user_id;
    return msg;
}

std::shared_ptr<MessageInterface> MessageBuilder::buildChatroomMessage() {
    auto msg = std::make_shared<ChatroomMessage>();
    msg->_success_bit = _success_bit;
    msg->_messageContents = _messageContents;
    msg->_from_user_name = _from_user_name;
    msg->_from_user_id = _from_user_id;
    msg->_to_chatroom_id = _to_chatroom_id;
    msg->_from_chatroom_id = _from_chatroom_id;
    return msg;
}

std::shared_ptr<MessageInterface> MessageBuilder::build() {
    switch (_messageType) {
        case MessageType::CONNECT:
            return buildConnectMessage();
        case MessageType::SEND_MESSAGE_TO_USER:
            return buildUserMessage();
        case MessageType::SEND_MESSAGE_TO_CHATROOM:
            return buildChatroomMessage();
        case MessageType::AUTHENTICATE:
            return buildAuthMessage();
        default:
            throw std::runtime_error("Unknown message type: " + 
                std::to_string(static_cast<int>(_messageType)));
    }
}

