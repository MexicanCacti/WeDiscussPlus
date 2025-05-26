#include "MessageFactory.hpp"
#include "AuthResponseMessage.hpp"

std::shared_ptr<MessageInterface> MessageFactory::deserialize(const std::vector<char>& data, size_t& offset) {
    if (data.empty() || offset >= data.size()) {
        throw std::runtime_error("Empty data buffer or invalid offset");
    }

    MessageType type = static_cast<MessageType>(MessageInterface::deserializeInt(data, offset));

    switch (type) {
        case MessageType::CONNECT:
            return std::make_shared<ConnectMessage>(data, offset);
        case MessageType::SEND_MESSAGE_TO_USER:
            return std::make_shared<UserMessage>(data, offset);
        case MessageType::SEND_MESSAGE_TO_CHATROOM:
            return std::make_shared<ChatroomMessage>(data, offset);
        case MessageType::AUTHENTICATE:
            return std::make_shared<AuthMessage>(data, offset);
        case MessageType::AUTH_RESPONSE:
            return std::make_shared<AuthResponseMessage>(data, offset);
        default:
            throw std::runtime_error("Unknown message type: " + std::to_string(static_cast<int>(type)));
    }
}
