#include "Message.hpp"

Message::Message(MessageBuilder* messageBuilder) 
    :   _messageContents(messageBuilder->getMessageContents()),
        _to_user_name(messageBuilder->getToUsername()),
        _to_user_id(messageBuilder->getToUserID()),
        _from_user_name(messageBuilder->getFromUsername()),
        _from_user_id(messageBuilder->getFromUserID()),
        _to_chatroom_id(messageBuilder->getToChatroomID()),
        _from_chatroom_id(messageBuilder->getFromChatroomID()),
        _messageType(messageBuilder->getMessageType()) {}


void Message::serializeInt(std::vector<char>& buffer, int val) {
    char* p = reinterpret_cast<char*>(&val);
    buffer.insert(buffer.end(), p, p + sizeof(int));
}
int  Message::deserializeInt(const std::vector<char>& buffer, size_t& offset){
    int val;
    std::memcpy(&val, buffer.data() + offset, sizeof(int));
    offset += sizeof(int);
    return val;
}

void Message::serializeString(std::vector<char>& buffer, const std::string& str) {
    serializeInt(buffer, static_cast<int>(str.length()));
    buffer.insert(buffer.end(), str.begin(), str.end());
}

std::string  Message::deserializeString(const std::vector<char>& buffer, size_t& offset){
    int len = deserializeInt(buffer, offset);
    std::string result(buffer.begin() + offset, buffer.begin() + offset + len);
    offset += len;
    return result;
}

std::vector<char> Message::serialize() const{
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

Message Message::deserialize(const std::vector<char>& data){
    Message message;
    size_t offset = 0;

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

std::string Message::messageTypeToString(const MessageType& messageType){
    switch(messageType){
        case MessageType::SEND:
            return "SEND";
        case MessageType::RECV:
            return "RECV";
        case MessageType::UNDEFINED:
            return "UNDEFINED";
        default:
            return "UNKNOWN";
    };
    return "ERROR";
}

void Message::printMessage(){
    std::cout << "Message Type: " << messageTypeToString(_messageType) << " Contents: " << _messageContents << std::endl;
}