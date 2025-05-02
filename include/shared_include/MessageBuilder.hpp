#include "shared_include/MessageBuilderInterface.hpp"
#include "shared_include/Message.hpp"
#ifndef MESSAGEBUILDER_HPP
#define MESSAGEBUILDER_HPP
class MessageBuilder : public MessageBuilderInterface{
    private:
        Message _message;

    public:
        MessageBuilder(){
            _message = Message();
        }

        void setMessageContents(std::string messageContents) override {
            _message.setMessageContents(messageContents);
        }

        void setToUsername(std::string username) override {
            _message.setToUsername(username);
        }

        void setToUserID(int userID) override {
            _message.setToUserID(userID);
        }

        void setFromUsername(std::string username) override {
            _message.setFromUsername(username);
        }

        void setFromUserID(int userID) override {
            _message.setFromUserID(userID);
        }

        void setToChatroomID(int chatroomID) override {
            _message.setToChatroomID(chatroomID);
        }

        void setFromChatroomID(int chatroomID) override{
            _message.setFromChatroomID(chatroomID);
        }

        void setMessageType(MessageType messageType){
            _message.setMessageType(messageType);
        }

        Message buildMessage() override{
            return _message;
        }
};


#endif