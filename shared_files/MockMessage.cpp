#include "MockMessage.hpp"

MockMessage::MockMessage(MessageBuilder<MockMessage>* messageBuilder) 
    :   _messageContents(messageBuilder->getMessageContents()),
        _to_user_name(messageBuilder->getToUsername()),
        _to_user_id(messageBuilder->getToUserID()),
        _from_user_name(messageBuilder->getFromUsername()),
        _from_user_id(messageBuilder->getFromUserID()),
        _to_chatroom_id(messageBuilder->getToChatroomID()),
        _from_chatroom_id(messageBuilder->getFromChatroomID()),
        _messageType(messageBuilder->getMessageType()) {}


void MockMessage::serializeInt(std::vector<char>& buffer, int val) {
    char* p = reinterpret_cast<char*>(&val);
    buffer.insert(buffer.end(), p, p + sizeof(int));
}
int  MockMessage::deserializeInt(const std::vector<char>& buffer, size_t& offset){
    int val;
    std::memcpy(&val, buffer.data() + offset, sizeof(int));
    offset += sizeof(int);
    return val;
}

void MockMessage::serializeString(std::vector<char>& buffer, const std::string& str) {
    serializeInt(buffer, static_cast<int>(str.length()));
    buffer.insert(buffer.end(), str.begin(), str.end());
}

std::string MockMessage::deserializeString(const std::vector<char>& buffer, size_t& offset){
    int len = deserializeInt(buffer, offset);
    std::string result(buffer.begin() + offset, buffer.begin() + offset + len);
    offset += len;
    return result;
}

std::vector<char> MockMessage::serialize() const{
    std::vector<char> buffer, content;

    serializeInt(content, static_cast<int>(_messageType));
    serializeInt(content, _from_user_id);
    serializeInt(content, _to_user_id);
    serializeInt(content, _from_chatroom_id);
    serializeInt(content, _to_chatroom_id);

    serializeString(content, _from_user_name);
    serializeString(content, _to_user_name);
    serializeString(content, _messageContents);

    serializeInt(buffer, static_cast<int>(content.size()));
    buffer.insert(buffer.end(), content.begin(), content.end());
    return buffer;
}

MockMessage MockMessage::deserialize(const std::vector<char>& data){
    MockMessage message;
    size_t offset = 0;

    // Now read the actual message content
    message._messageType = static_cast<MessageType>(deserializeInt(data, offset));
    message._from_user_id = deserializeInt(data, offset);
    message._to_user_id = deserializeInt(data, offset);
    message._from_chatroom_id = deserializeInt(data, offset);
    message._to_chatroom_id = deserializeInt(data, offset);

    message._from_user_name = deserializeString(data, offset);
    message._to_user_name = deserializeString(data, offset);
    message._messageContents = deserializeString(data, offset);

    return message;
}

std::string MockMessage::messageTypeToString(const MessageType& messageType){
    switch(messageType){
        case MessageType::LOGOUT:
            return "LOGOUT";
        case MessageType::ADD_USER:
            return "ADD_USER";
        case MessageType::CHANGE_USER_PASSWORD:
            return "CHANGE_USER_PASSWORD";
        case MessageType::CHANGE_USER_NAME:
            return "CHANGE_USER_NAME";
        case MessageType::DELETE_USER:
            return "DELETE_USER";
        case MessageType::SEND_MESSAGE_TO_USER:
            return "SEND_MESSAGE_TO_USER";
        case MessageType::CREATE_CHATROOM:
            return "CREATE_CHATROOM";
        case MessageType::DELETE_CHATROOM:
            return "DELETE_CHATROOM";
        case MessageType::ADD_USER_TO_CHATROOM:
            return "ADD_USER_TO_CHATROOM";
        case MessageType::SEND_MESSAGE_TO_CHATROOM:
            return "SEND_MESSAGE_TO_CHATROOM";
        case MessageType::REMOVE_USER_FROM_CHATROOM:
            return "REMOVE_USER_FROM_CHATROOM";
        case MessageType::GET_USER_MESSAGES:
            return "GET_USER_MESSAGES";
        case MessageType::GET_CHATROOM_MESSAGES:
            return "GET_CHATROOM_MESSAGES";
        case MessageType::SEND:
            return "SEND";
        case MessageType::RECV:
            return "RECV";
        case MessageType::ACK:
            return "ACK";
        case MessageType::UNDEFINED:
            return "UNDEFINED";
        case MessageType::TEST:
            return "TEST";
        default:
            return "UNKNOWN";
    }
}

void MockMessage::printMessage() const{
    std::cout << "Message Type: " << messageTypeToString(_messageType) << " Contents: " << _messageContents << std::endl;
}