#pragma once

#include "MessageInterface.hpp"
#include "MessageFactory.hpp"

// Chatroom communication
class LogRequestMessage : public MessageInterface {
    private:
        std::vector<std::shared_ptr<MessageInterface>> _messages;
    public:
        LogRequestMessage() = default;
        LogRequestMessage(const std::vector<char>& data, size_t& offset);
        void setMessages(const std::vector<std::shared_ptr<MessageInterface>>& messages);
        const std::vector<std::shared_ptr<MessageInterface>>& getMessages() const { return _messages; }
        std::vector<char> serialize() const override;
        void deserialize(const std::vector<char>& data, size_t& offset) override;
        void printMessage() const override;
};
