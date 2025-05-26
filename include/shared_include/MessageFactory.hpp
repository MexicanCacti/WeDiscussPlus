#pragma once
#include "MessageInterface.hpp"
#include "ConnectMessage.hpp"
#include "UserMessage.hpp"
#include "ChatroomMessage.hpp"
#include "AuthMessage.hpp"
#include <stdexcept>

class MessageFactory {
public:
    static std::shared_ptr<MessageInterface> deserialize(const std::vector<char>& data, size_t& offset);
};