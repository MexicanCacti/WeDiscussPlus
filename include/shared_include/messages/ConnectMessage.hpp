#pragma once

#include "MessageInterface.hpp"

// Initial connection and authentication
class ConnectMessage : public MessageInterface {
    public:
        ConnectMessage() = default;

        ConnectMessage(const std::vector<char>& data, size_t& offset);

        std::vector<char> serialize() const override;

        void deserialize(const std::vector<char>& data, size_t& offset);

        void printMessage() const override;
    };