#include "MessageBuilder.hpp"
#include "MessageInterface.hpp"
#include "Message.hpp"
#include "MockMessage.hpp"
#include "shared_include/User.hpp"
#include "shared_include/Chatroom.hpp"

template<typename MessageObject>
MessageBuilder<MessageObject>::MessageBuilder(MessageObject* message)
    :   _messageContents(message->getMessageContents()),
        _to_user_name(message->getToUsername()),
        _to_user_id(message->getToUserID()),
        _from_user_name(message->getFromUsername()),
        _from_user_id(message->getFromUserID()),
        _to_chatroom_id(message->getToChatroomID()),
        _from_chatroom_id(message->getFromChatroomID()),
        _messageType(message->getMessageType()) {}

template<typename MessageObject>
MessageObject MessageBuilder<MessageObject>::buildMessage(){
    return MessageObject(this);
}

template class MessageBuilder<Message>;
template class MessageBuilder<MockMessage>;
    