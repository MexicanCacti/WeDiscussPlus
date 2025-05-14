#include "Message.hpp"
#include "shared_include/User.hpp"
#include "shared_include/Chatroom.hpp"

Message::Message(MessageBuilder<Message>* messageBuilder) 
    : MessageInterface() {
    _success_bit = messageBuilder->getSuccessBit();
    _messageContents = messageBuilder->getMessageContents();
    _to_user_name = messageBuilder->getToUsername();
    _to_user_id = messageBuilder->getToUserID();
    _from_user_name = messageBuilder->getFromUsername();
    _from_user_id = messageBuilder->getFromUserID();
    _to_chatroom_id = messageBuilder->getToChatroomID();
    _from_chatroom_id = messageBuilder->getFromChatroomID();
    _messageType = messageBuilder->getMessageType();
    _user = messageBuilder->getUser();
    _chatroom = messageBuilder->getChatroom();
    _chatrooms = messageBuilder->getChatrooms();
    _userIDToUsername = messageBuilder->getUserIDToUsername();
}

std::vector<char> Message::serialize() const {
    std::vector<char> buffer, content;

    serializeInt(content, _success_bit);
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

Message Message::deserialize(const std::vector<char>& data) {
    Message message;
    size_t offset = 0;

    message._success_bit = deserializeInt(data, offset);
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

void Message::printMessage() const {
    std::cout << "Message Type: " << messageTypeToString(_messageType) << " Contents: " << _messageContents << std::endl;
}