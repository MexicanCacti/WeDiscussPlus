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
        Message(): _messageContents(""), _to_user_name(""), _to_user_id(-1), _from_user_name(""), _from_user_id(-1), _to_chatroom_id(-1), _from_chatroom_id(-1), _messageType(MessageType::UNDEFINED){};

        void setMessageContents(std::string messageContents){
            _messageContents = std::move(messageContents);
        }

        void setToUsername(std::string username){
            _to_user_name = std::move(username);
        }

        void setToUserID(int userID){
            _to_user_id = userID;
        }

        void setFromUsername(std::string username){
            _from_user_name = std::move(username);
        }

        void setFromUserID(int userID){
            _from_user_id = userID;
        }

        void setToChatroomID(int chatroomID){
            _to_chatroom_id = chatroomID;
        }

        void setFromChatroomID(int chatroomID){
            _from_chatroom_id = chatroomID;
        }

        void setMessageType(MessageType messageType){
            _messageType = std::move(messageType);
        }
};

#endif