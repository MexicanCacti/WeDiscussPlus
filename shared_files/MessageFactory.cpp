#include "MessageFactory.hpp"

std::shared_ptr<MessageInterface> MessageFactory::deserialize(const std::vector<char>& data, size_t& offset) {
    if (data.empty() || offset >= data.size()) {
        throw std::runtime_error("Empty data buffer or invalid offset");
    }

    MessageType type = static_cast<MessageType>(MessageInterface::deserializeInt(data, offset));
    #ifndef _DEBUG
        std::cout << "Deserialized message type: " << MessageInterface::messageTypeToString(type) << std::endl;
    #endif
    std::shared_ptr<MessageInterface> message;

    switch (type) {
        case MessageType::CONNECT:
            message = std::make_shared<ConnectMessage>(data, offset);
            break;
        case MessageType::SEND_MESSAGE_TO_USER:
        case MessageType::ADD_USER:
        case MessageType::CHANGE_USER_PASSWORD:
        case MessageType::CHANGE_USER_NAME:
        case MessageType::DELETE_USER:
        case MessageType::LOGOUT:
            message = std::make_shared<UserMessage>(data, offset);
            break;
        case MessageType::SEND_MESSAGE_TO_CHATROOM:
        case MessageType::ADD_USER_TO_CHATROOM:
        case MessageType::REMOVE_USER_FROM_CHATROOM:
        case MessageType::CREATE_CHATROOM:
        case MessageType::DELETE_CHATROOM:
            message = std::make_shared<ChatroomMessage>(data, offset);
            break;
        case MessageType::AUTHENTICATE:
            message = std::make_shared<AuthMessage>(data, offset);
            break;
        case MessageType::AUTH_RESPONSE:
            message = std::make_shared<AuthResponseMessage>(data, offset);
            break;
        case MessageType::GET_TO_USER_MESSAGES:
        case MessageType::GET_FROM_USER_MESSAGES:
        case MessageType::GET_BETWEEN_USERS_MESSAGES:
        case MessageType::GET_CHATROOM_MESSAGES:
        case MessageType::GET_CHATROOM_MESSAGES_FROM_USER:
            message = std::make_shared<LogRequestMessage>(data, offset);
            break;
        default:
            throw std::runtime_error("Unknown message type: " + std::to_string(static_cast<int>(type)));
    }

    // Set the message type after creating the message
    message->_messageType = type;
    return message;
}
