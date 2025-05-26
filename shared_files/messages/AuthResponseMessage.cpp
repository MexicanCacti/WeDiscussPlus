#include "AuthResponseMessage.hpp"

AuthResponseMessage::AuthResponseMessage(const std::vector<char>& data, size_t& offset) {
    _messageType = MessageType::AUTH_RESPONSE;
    deserialize(data, offset);
}

void AuthResponseMessage::setUserMap(const std::unordered_map<int, std::string>& userMap) {
    _userMap = userMap;
}

void AuthResponseMessage::setChatrooms(const std::vector<ChatroomData>& chatrooms) {
    _chatrooms = chatrooms;
}

void AuthResponseMessage::setInbox(const std::vector<std::shared_ptr<MessageInterface>>& messages) {
    _inbox = messages;
}

std::vector<char> AuthResponseMessage::serialize() const {
    std::vector<char> buffer;

    serializeInt(buffer, static_cast<int>(_messageType));
    
    serializeInt(buffer, _success_bit ? 1 : 0);
    serializeInt(buffer, static_cast<int>(_to_user_id));
    serializeString(buffer, _to_user_name);

    serializeInt(buffer, static_cast<int>(_userMap.size()));
    for (const auto& [id, name] : _userMap) {
        serializeInt(buffer, id);
        serializeString(buffer, name);
    }
    
    serializeInt(buffer, static_cast<int>(_chatrooms.size()));
    for (const auto& chatroom : _chatrooms) {
        serializeInt(buffer, chatroom.id);
        
        serializeInt(buffer, static_cast<int>(chatroom.memberIds.size()));
        for (int memberId : chatroom.memberIds) {
            serializeInt(buffer, memberId);
        }
        
        serializeInt(buffer, static_cast<int>(chatroom.messages.size()));
        for (const auto& msg : chatroom.messages) {
            std::vector<char> msgBuffer = msg->serialize();
            buffer.insert(buffer.end(), msgBuffer.begin(), msgBuffer.end());
        }
    }
    
    serializeInt(buffer, static_cast<int>(_inbox.size()));
    for (const auto& msg : _inbox) {
        std::vector<char> msgBuffer = msg->serialize();
        buffer.insert(buffer.end(), msgBuffer.begin(), msgBuffer.end());
    }

    return buffer;
}

void AuthResponseMessage::deserialize(const std::vector<char>& data, size_t& offset) {
    _success_bit = deserializeInt(data, offset) == 1;
    _to_user_id = deserializeInt(data, offset);
    _to_user_name = deserializeString(data, offset);
    
    int userMapSize = deserializeInt(data, offset);
    for (int i = 0; i < userMapSize; i++) {
        int id = deserializeInt(data, offset);
        std::string name = deserializeString(data, offset);
        _userMap[id] = name;
    }
    
    int chatroomCount = deserializeInt(data, offset);
    _chatrooms.resize(chatroomCount);
    for (auto& chatroom : _chatrooms) {
        chatroom.id = deserializeInt(data, offset);
        
        int memberCount = deserializeInt(data, offset);
        chatroom.memberIds.resize(memberCount);
        for (int& memberId : chatroom.memberIds) {
            memberId = deserializeInt(data, offset);
        }
        
        int messageCount = deserializeInt(data, offset);
        chatroom.messages.resize(messageCount);
        for (auto& msg : chatroom.messages) {
            msg = MessageFactory::deserialize(data, offset);
        }
    }
    
    int inboxSize = deserializeInt(data, offset);
    _inbox.resize(inboxSize);
    for (auto& msg : _inbox) {
        msg = MessageFactory::deserialize(data, offset);
    }
}

void AuthResponseMessage::printMessage() const {
    std::cout << "Authentication Response Message:\n"
              << "  Success: " << (_success_bit ? "Yes" : "No") << "\n"
              << "  User Map Size: " << _userMap.size() << "\n"
              << "  Chatrooms: " << _chatrooms.size() << "\n"
              << "  Inbox Messages: " << _inbox.size() << "\n"
              << "  To User ID: " << _to_user_id << "\n"
              << "  To User Name: " << _to_user_name << "\n";
} 