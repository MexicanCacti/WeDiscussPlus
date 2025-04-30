#include "MessageBuilder.hpp"
#include "Message.hpp"

#ifndef MESSAGECREATOR_HPP
#define MESSAGECREATOR_HPP

class MessageCreator {
    public:
        Message buildMessage(MessageBuilder& messageBuilder){
            return messageBuilder.buildMessage();
        }
};

#endif