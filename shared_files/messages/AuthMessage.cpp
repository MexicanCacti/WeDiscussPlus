#include "AuthMessage.hpp"

AuthMessage::AuthMessage(const std::vector<char>& data, size_t& offset) {
    _messageType = MessageType::AUTHENTICATE;
    deserialize(data, offset);
}

std::vector<char> AuthMessage::serialize() const {
    std::vector<char> buffer;
    serializeInt(buffer, static_cast<int>(_messageType));
    serializeInt(buffer, _success_bit ? 1 : 0);
    serializeString(buffer, _messageContents);
    serializeString(buffer, _to_user_name);
    serializeInt(buffer, _to_user_id);
    return buffer;
}

void AuthMessage::deserialize(const std::vector<char>& data, size_t& offset) {
    _success_bit = deserializeInt(data, offset) == 1;
    _messageContents = deserializeString(data, offset);
    _to_user_name = deserializeString(data, offset);
    _to_user_id = deserializeInt(data, offset);
}

void AuthMessage::printMessage() const {
    std::cout << "Authentication Message:\n"
              << "  Success: " << (_success_bit ? "Yes" : "No") << "\n"
              << "  Contents: " << _messageContents << "\n"
              << "  User: " << _to_user_name << " (ID: " << _to_user_id << ")\n";
} 