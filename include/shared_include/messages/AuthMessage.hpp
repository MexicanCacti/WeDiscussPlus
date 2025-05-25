#pragma once

#include "MessageInterface.hpp"

class AuthMessage : public MessageInterface {
public:
    AuthMessage() {_messageType = MessageType::AUTHENTICATE;}

    AuthMessage(const std::vector<char>& data, size_t& offset);

    std::vector<char> serialize() const override;

    void deserialize(const std::vector<char>& data, size_t& offset);

    void printMessage() const override;
};
