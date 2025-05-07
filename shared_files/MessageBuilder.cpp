#include "shared_include/MessageBuilder.hpp"
#include "shared_include/Message.hpp"

MessageBuilder::MessageBuilder(Message* message)
    :   _messageContents(message->getMessageContents()),
        _to_user_name(message->getToUsername()),
        _to_user_id(message->getToUserID()),
        _from_user_name(message->getFromUsername()),
        _from_user_id(message->getFromUserID()),
        _to_chatroom_id(message->getToChatroomID()),
        _from_chatroom_id(message->getFromChatroomID()),
        _messageType(message->getMessageType()) {}

Message MessageBuilder::buildMessage(){
    return Message(this);
}