#pragma once

#include "MessageInterface.hpp"

struct UserData {
    int id;
    std::string username;
    std::vector<std::shared_ptr<MessageInterface>> messages;
};

// User-to-user communication
class UserMessage : public MessageInterface {
    public:
        UserMessage() = default;
        UserMessage(const std::vector<char>& data, size_t& offset);
        
        std::vector<char> serialize() const override;
        void deserialize(const std::vector<char>& data, size_t& offset) override;
        void printMessage() const override;
};