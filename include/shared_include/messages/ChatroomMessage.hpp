#pragma once

#include "MessageInterface.hpp"

// Data to be sent to the client
struct ChatroomData {
    int id;
    std::vector<int> memberIds;
    std::vector<std::shared_ptr<MessageInterface>> messages;
};

// Chatroom communication
class ChatroomMessage : public MessageInterface {
    public:
        ChatroomMessage() = default;
        ChatroomMessage(const std::vector<char>& data, size_t& offset);
        
        std::vector<char> serialize() const override;
        void deserialize(const std::vector<char>& data, size_t& offset) override;
        void printMessage() const override;
};
