#include "LogRequestMessage.hpp"
#include <iostream>

#include "ChatroomMessage.hpp"
#include <iostream>

LogRequestMessage::LogRequestMessage(const std::vector<char>& data, size_t& offset) {
    deserialize(data, offset);
}

void LogRequestMessage::setMessages(const std::vector<std::shared_ptr<MessageInterface>>& messages) {
    _messages = messages;
}

std::vector<char> LogRequestMessage::serialize() const {
    std::vector<char> buffer;

    serializeInt(buffer, static_cast<int>(_messageType));
    
    serializeInt(buffer, _success_bit ? 1 : 0);
    serializeInt(buffer, static_cast<int>(_to_user_id));
    serializeString(buffer, _to_user_name);
    serializeInt(buffer, _from_user_id);
    serializeString(buffer, _from_user_name);
    serializeInt(buffer, _to_chatroom_id);
    serializeString(buffer, _messageContents);

    serializeInt(buffer, static_cast<int>(_messages.size()));
    for (const auto& msg : _messages) {
        std::vector<char> msgBuffer = msg->serialize();
        buffer.insert(buffer.end(), msgBuffer.begin(), msgBuffer.end());
    }


    return buffer;
}

void LogRequestMessage::deserialize(const std::vector<char>& data, size_t& offset) {
    _success_bit = deserializeInt(data, offset) == 1;
    _to_user_id = deserializeInt(data, offset);
    _to_user_name = deserializeString(data, offset);
    _from_user_id = deserializeInt(data, offset);
    _from_user_name = deserializeString(data, offset);
    _to_chatroom_id = deserializeInt(data, offset);
    _messageContents = deserializeString(data, offset);
    int messageCount = deserializeInt(data, offset);
    _messages.resize(messageCount);
    for (int i = 0; i < messageCount; i++) {
        _messages[i] = MessageFactory::deserialize(data, offset);
    }
}

void LogRequestMessage::printMessage() const {
    std::cout << "Chatroom Message:\n"
              << "  Success: " << (_success_bit ? "Yes" : "No") << "\n"
              << "  To User: " << _to_user_name << " (ID: " << _to_user_id << ")\n"
              << "  From User: " << _from_user_name << " (ID: " << _from_user_id << ")\n"
              << "  To Chatroom: " << _to_chatroom_id << "\n"
              << "  Message Contents: " << _messageContents << "\n";
} 