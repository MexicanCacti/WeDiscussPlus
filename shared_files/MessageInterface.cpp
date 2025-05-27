#include "MessageInterface.hpp"
#include <cstring>
#include <iostream>

void MessageInterface::serializeInt(std::vector<char>& buffer, int val) {
    char* p = reinterpret_cast<char*>(&val); // Geting ptr to first byte of the val
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
        case MessageType::CONNECT:
            return "CONNECT";
        case MessageType::AUTHENTICATE:
            return "AUTHENTICATE";
        case MessageType::AUTH_RESPONSE:
            return "AUTH_RESPONSE";
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
        case MessageType::GET_TO_USER_MESSAGES:
            return "GET_TO_USER_MESSAGES";
        case MessageType::GET_FROM_USER_MESSAGES:
            return "GET_FROM_USER_MESSAGES";
        case MessageType::GET_BETWEEN_USERS_MESSAGES:
            return "GET_BETWEEN_USERS_MESSAGES";
        case MessageType::GET_CHATROOM_MESSAGES:
            return "GET_CHATROOM_MESSAGES";
        case MessageType::GET_CHATROOM_MESSAGES_FROM_USER:
            return "GET_CHATROOM_MESSAGES_FROM_USER";
        case MessageType::STORE_CONNECT_LOG:
            return "STORE_CONNECT_LOG";
        case MessageType::UNDEFINED:
            return "UNDEFINED";
        case MessageType::TEST:
            return "TEST";
        default:
            return "UNKNOWN";
    }
}