#pragma once
#include "MessageInterface.hpp"
#include "messages/ConnectMessage.hpp"
#include "messages/UserMessage.hpp"
#include "messages/ChatroomMessage.hpp"
#include "messages/AuthMessage.hpp"
#include "messages/AuthResponseMessage.hpp"
#include "messages/LogRequestMessage.hpp"
#include <stdexcept>

class MessageFactory {
public:
    static std::shared_ptr<MessageInterface> deserialize(const std::vector<char>& data, size_t& offset);
};