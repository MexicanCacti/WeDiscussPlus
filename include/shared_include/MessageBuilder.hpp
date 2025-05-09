
#ifndef MESSAGEBUILDER_HPP
#define MESSAGEBUILDER_HPP

class Message;

#include "shared_include/MessageType.hpp"
#include <string>

class MessageBuilder {
    private:
        std::string _messageContents;
        std::string _to_user_name;
        int _to_user_id;
        std::string _from_user_name;
        int _from_user_id;
        int _to_chatroom_id;
        int _from_chatroom_id;
        MessageType _messageType;

    public:
        MessageBuilder() = default;
        MessageBuilder(Message* message);

        inline void setMessageContents(const std::string& messageContents) {_messageContents = messageContents;}
        inline const std::string& getMessageContents() const {return _messageContents;}
        
        inline void setToUsername(const std::string& username) {_to_user_name = username;}
        inline const std::string& getToUsername() const {return _to_user_name;}
        
        inline void setToUserID(int userID) {_to_user_id = userID;}
        inline int getToUserID() const {return _to_user_id;}
        
        inline void setFromUsername(const std::string& username) {_from_user_name = username;}
        inline const std::string& getFromUsername() const {return _from_user_name;}
        
        inline void setFromUserID(int userID) {_from_user_id = userID;}
        inline int getFromUserID() const {return _from_user_id;}
        
        inline void setToChatroomID(int chatroomID) {_to_chatroom_id = chatroomID;}
        inline int getToChatroomID() const {return _to_chatroom_id;}
        
        inline void setFromChatroomID(int chatroomID) {_from_chatroom_id = chatroomID;}
        inline int getFromChatroomID() const {return _from_chatroom_id;}
        
        inline void setMessageType(MessageType messageType) {_messageType = messageType;}
        inline MessageType getMessageType() const {return _messageType;}   

        Message buildMessage();
};

#endif