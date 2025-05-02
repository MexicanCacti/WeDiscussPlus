#include "shared_include/Message.hpp"

#ifndef MESSAGEBUILDERINTERFACE_HPP
#define MESSAGEBUILDERINTERFACE_HPP

class MessageBuilderInterface{
    public:
        // Possibly consider adding bool/int return type to indicate success/failure of setting
        virtual void setMessageContents(std::string messageContents) = 0;

        virtual void setToUsername(std::string username) = 0;

        virtual void setToUserID(int userID) = 0;

        virtual void setFromUsername(std::string username) = 0;

        virtual void setFromUserID(int userID) = 0;

        virtual void setToChatroomID(int chatroomID) = 0;

        virtual void setFromChatroomID(int chatroomID) = 0;

        virtual void setMessageType(MessageType messageType) = 0;

        virtual Message buildMessage() = 0;
};
#endif