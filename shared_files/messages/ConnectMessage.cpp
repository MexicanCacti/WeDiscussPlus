#include "ConnectMessage.hpp"

ConnectMessage::ConnectMessage(const std::vector<char>& data, size_t& offset) {
    deserialize(data, offset);
}

std::vector<char> ConnectMessage::serialize() const {
    std::vector<char> buffer;
    serializeInt(buffer, static_cast<int>(_messageType));
    serializeInt(buffer, _success_bit ? 1 : 0);
    serializeString(buffer, _messageContents);
    serializeString(buffer, _to_user_name);
    serializeInt(buffer, _to_user_id);
    serializeString(buffer, _from_user_name);
    serializeInt(buffer, _from_user_id);
    return buffer;
}

void ConnectMessage::deserialize(const std::vector<char>& data, size_t& offset) {
    _success_bit = deserializeInt(data, offset);
    _messageContents = deserializeString(data, offset);
    _to_user_name = deserializeString(data, offset);
    _to_user_id = deserializeInt(data, offset);
    _from_user_name = deserializeString(data, offset);
    _from_user_id = deserializeInt(data, offset);
}

void ConnectMessage::printMessage() const {
    std::cout << "Connect Message:\n"
              << "  Success: " << (_success_bit ? "Yes" : "No") << "\n"
              << "  Contents: " << _messageContents << "\n"
              << "  To User: " << _to_user_name << " (ID: " << _to_user_id << ")\n"
              << "  From User: " << _from_user_name << " (ID: " << _from_user_id << ")\n";
}