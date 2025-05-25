#pragma once

#include "MessageInterface.hpp"

// User-to-user communication
class UserMessage : public MessageInterface {
public:
    UserMessage();
    UserMessage(const std::vector<char>& data, size_t& offset);
    
    std::vector<char> serialize() const override;
    void deserialize(const std::vector<char>& data, size_t& offset) override;
    void printMessage() const override;
};