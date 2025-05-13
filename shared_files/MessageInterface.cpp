#include "MessageInterface.hpp"
#include <cstring>

void MessageInterface::serializeInt(std::vector<char>& buffer, int val) {
    char* p = reinterpret_cast<char*>(&val);
    buffer.insert(buffer.end(), p, p + sizeof(int));
}

int MessageInterface::deserializeInt(const std::vector<char>& buffer, size_t& offset) {
    int val;
    std::memcpy(&val, buffer.data() + offset, sizeof(int));
    offset += sizeof(int);
    return val;
}

void MessageInterface::serializeString(std::vector<char>& buffer, const std::string& str) {
    serializeInt(buffer, static_cast<int>(str.length()));
    buffer.insert(buffer.end(), str.begin(), str.end());
}

std::string MessageInterface::deserializeString(const std::vector<char>& buffer, size_t& offset) {
    int len = deserializeInt(buffer, offset);
    std::string result(buffer.begin() + offset, buffer.begin() + offset + len);
    offset += len;
    return result;
} 

std::string MessageInterface::messageTypeToString(const MessageType& messageType) {
    switch(messageType) {
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
        case MessageType::DENIED:
            return "DENIED";
        default:
            return "UNKNOWN";
    }
}