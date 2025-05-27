#include "MessageBuilder.hpp"

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

void MessageBuilder::setUserMap(const std::unordered_map<int, std::string>& userMap) {
    _userMap = userMap;
}

void MessageBuilder::setChatrooms(const std::vector<ChatroomData>& chatrooms) {
    _chatrooms = chatrooms;
}   

void MessageBuilder::setInbox(const std::vector<std::shared_ptr<MessageInterface>>& inbox) {
    _inbox = inbox;
}

std::shared_ptr<MessageInterface> MessageBuilder::buildConnectMessage() {
    auto msg = std::make_shared<ConnectMessage>();
    msg->_messageType = MessageType::CONNECT;
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
    msg->_messageType = MessageType::AUTHENTICATE;
    msg->_success_bit = _success_bit;
    msg->_messageContents = _messageContents;
    msg->_to_user_name = _to_user_name;
    msg->_to_user_id = _to_user_id;
    msg->_from_user_name = _from_user_name;
    msg->_from_user_id = _from_user_id;
    return msg;
}

std::shared_ptr<MessageInterface> MessageBuilder::buildAuthResponseMessage() {
    auto msg = std::make_shared<AuthResponseMessage>();
    msg->_messageType = MessageType::AUTH_RESPONSE;
    msg->_success_bit = _success_bit;
    msg->_messageContents = _messageContents;
    msg->_to_user_name = _to_user_name;
    msg->_to_user_id = _to_user_id;
    msg->setUserMap(_userMap);
    msg->setChatrooms(_chatrooms);
    msg->setInbox(_inbox);
    return msg;
}

std::shared_ptr<MessageInterface> MessageBuilder::buildLogoutMessage() {
    auto msg = std::make_shared<AuthMessage>();
    msg->_messageType = MessageType::LOGOUT;
    msg->_success_bit = _success_bit;
    msg->_messageContents = _messageContents;
    msg->_from_user_name = _from_user_name;
    msg->_from_user_id = _from_user_id;
    return msg;
}

std::shared_ptr<MessageInterface> MessageBuilder::buildUserMessage() {
    auto msg = std::make_shared<UserMessage>();
    msg->_messageType = _messageType;
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
    msg->_messageType = _messageType;
    msg->_success_bit = _success_bit;
    msg->_messageContents = _messageContents;
    msg->_from_user_name = _from_user_name;
    msg->_from_user_id = _from_user_id;
    msg->_to_chatroom_id = _to_chatroom_id;
    msg->_from_chatroom_id = _from_chatroom_id;
    return msg;
}

std::shared_ptr<MessageInterface> MessageBuilder::buildLogRequestMessage() {
    auto msg = std::make_shared<LogRequestMessage>();
    msg->_messageType = _messageType;
    msg->_success_bit = _success_bit;
    msg->_messageContents = _messageContents;
    msg->_from_user_name = _from_user_name;
    msg->_from_user_id = _from_user_id;
    msg->_to_user_name = _to_user_name;
    msg->_to_user_id = _to_user_id;
    msg->_to_chatroom_id = _to_chatroom_id;
    return msg;
}

std::shared_ptr<MessageInterface> MessageBuilder::build() {
    switch (_messageType) {
        case MessageType::CONNECT:
            return buildConnectMessage();
        case MessageType::AUTHENTICATE:
            return buildAuthMessage();
        case MessageType::AUTH_RESPONSE:
            return buildAuthResponseMessage();
        case MessageType::LOGOUT:
            return buildLogoutMessage();
        case MessageType::SEND_MESSAGE_TO_USER:
        case MessageType::ADD_USER:
        case MessageType::CHANGE_USER_PASSWORD:
        case MessageType::CHANGE_USER_NAME:
        case MessageType::DELETE_USER:
            return buildUserMessage();
        case MessageType::SEND_MESSAGE_TO_CHATROOM:
        case MessageType::ADD_USER_TO_CHATROOM:
        case MessageType::REMOVE_USER_FROM_CHATROOM:
        case MessageType::CREATE_CHATROOM:
        case MessageType::DELETE_CHATROOM:
            return buildChatroomMessage();
        case MessageType::GET_TO_USER_MESSAGES:
        case MessageType::GET_FROM_USER_MESSAGES:
        case MessageType::GET_BETWEEN_USERS_MESSAGES:
        case MessageType::GET_CHATROOM_MESSAGES:
        case MessageType::GET_CHATROOM_MESSAGES_FROM_USER:
            return buildLogRequestMessage();
        default:
            throw std::runtime_error("Unknown message type: " + 
                std::to_string(static_cast<int>(_messageType)));
    }
}

