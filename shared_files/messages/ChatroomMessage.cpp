#include "ChatroomMessage.hpp"
#include <iostream>

ChatroomMessage::ChatroomMessage() {
    _messageType = MessageType::SEND_MESSAGE_TO_CHATROOM;
}

ChatroomMessage::ChatroomMessage(const std::vector<char>& data, size_t& offset) {
    _messageType = MessageType::SEND_MESSAGE_TO_CHATROOM;
    deserialize(data, offset);
}

std::vector<char> ChatroomMessage::serialize() const {
    std::vector<char> buffer;
    serializeInt(buffer, static_cast<int>(_messageType));
    serializeInt(buffer, _success_bit ? 1 : 0);
    serializeString(buffer, _messageContents);
    serializeString(buffer, _from_user_name);
    serializeInt(buffer, _from_user_id);
    serializeInt(buffer, _to_chatroom_id);
    return buffer;
}

void ChatroomMessage::deserialize(const std::vector<char>& data, size_t& offset) {
    _success_bit = deserializeInt(data, offset) == 1;
    _messageContents = deserializeString(data, offset);
    _from_user_name = deserializeString(data, offset);
    _from_user_id = deserializeInt(data, offset);
    _to_chatroom_id = deserializeInt(data, offset);
}

void ChatroomMessage::printMessage() const {
    std::cout << "Chatroom Message:\n"
              << "  Success: " << (_success_bit ? "Yes" : "No") << "\n"
              << "  Contents: " << _messageContents << "\n"
              << "  From User: " << _from_user_name << " (ID: " << _from_user_id << ")\n"
              << "  To Chatroom: " << _to_chatroom_id << "\n";
} 