#include "MockMessage.hpp"

MockMessage::MockMessage(MessageBuilder<MockMessage>* messageBuilder) 
    : MessageInterface() {
    _messageContents = messageBuilder->getMessageContents();
    _to_user_name = messageBuilder->getToUsername();
    _to_user_id = messageBuilder->getToUserID();
    _from_user_name = messageBuilder->getFromUsername();
    _from_user_id = messageBuilder->getFromUserID();
    _to_chatroom_id = messageBuilder->getToChatroomID();
    _from_chatroom_id = messageBuilder->getFromChatroomID();
    _messageType = messageBuilder->getMessageType();
}

std::vector<char> MockMessage::serialize() const {
    std::vector<char> buffer, content;

    serializeInt(content, static_cast<int>(_messageType));
    serializeInt(content, _from_user_id);
    serializeInt(content, _to_user_id);
    serializeInt(content, _from_chatroom_id);
    serializeInt(content, _to_chatroom_id);

    serializeString(content, _from_user_name);
    serializeString(content, _to_user_name);
    serializeString(content, _messageContents);

    serializeInt(buffer, static_cast<int>(content.size()));
    buffer.insert(buffer.end(), content.begin(), content.end());
    return buffer;
}

MockMessage MockMessage::deserialize(const std::vector<char>& data) {
    MockMessage message;
    size_t offset = 0;

    message._messageType = static_cast<MessageType>(deserializeInt(data, offset));
    message._from_user_id = deserializeInt(data, offset);
    message._to_user_id = deserializeInt(data, offset);
    message._from_chatroom_id = deserializeInt(data, offset);
    message._to_chatroom_id = deserializeInt(data, offset);

    message._from_user_name = deserializeString(data, offset);
    message._to_user_name = deserializeString(data, offset);
    message._messageContents = deserializeString(data, offset);

    return message;
}

void MockMessage::printMessage() const {
    std::cout << "Message Type: " << messageTypeToString(_messageType) << " Contents: " << _messageContents << std::endl;
}