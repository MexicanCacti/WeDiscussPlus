#pragma once

#include "MessageInterface.hpp"
#include <memory>

// Chatroom communication
class ChatroomMessage : public MessageInterface {
public:
    ChatroomMessage();
    ChatroomMessage(const std::vector<char>& data, size_t& offset);
    
    std::vector<char> serialize() const override;
    void deserialize(const std::vector<char>& data, size_t& offset) override;
    void printMessage() const override;
};
