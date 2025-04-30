#include "MessageType.hpp"
#include <string>

#ifndef MESSAGE_HPP
#define MESSAGE_HPP
class Message{
    private:
        std::string _messageContents;
        std::string _to_user_name;
        int _to_user_id;
        std::string _from_user_name;
        int _from_user_id;
        int _to_chatroom_id;
        int _from_chatroom_id;
        MessageType _messageType;
        //User _user
        //Chatroom _chatroom
        //Atomic-safe hashmap of ChatroomID: ChatroomObject
        //Atomic-safe hashmap of UserID : UserName
    public:
        inline Message(): _messageContents(""), _to_user_name(""), _to_user_id(-1), _from_user_name(""), _from_user_id(-1), _to_chatroom_id(-1), _from_chatroom_id(-1), _messageType(MessageType::UNDEFINED){};

        inline void setMessageContents(std::string messageContents) {_messageContents = std::move(messageContents);}

        inline std::string getMessageContents() const {return _messageContents;}

        void setToUsername(std::string username) {_to_user_name = std::move(username);}

        inline std::string getToUsername() const {return _to_user_name;}

        inline void setToUserID(int userID) {_to_user_id = userID;}

        inline int getToUserID() const {return _to_user_id;}

        inline void setFromUsername(std::string username) {_from_user_name = std::move(username);}

        inline std::string getFromUsername() const {return _from_user_name;}

        inline void setFromUserID(int userID) {_from_user_id = userID;}

        inline int getFromUserID() const {return _from_user_id;}

        inline void setToChatroomID(int chatroomID) {_to_chatroom_id = chatroomID;}

        inline int getToChatroomID() const {return _to_chatroom_id;}

        inline void setFromChatroomID(int chatroomID) {_from_chatroom_id = chatroomID;}

        inline int getFromChatroomID() const {return _from_chatroom_id;}

        inline void setMessageType(MessageType messageType) {_messageType = std::move(messageType);}

        inline MessageType getMessageType() const {return _messageType;}
};

#endif