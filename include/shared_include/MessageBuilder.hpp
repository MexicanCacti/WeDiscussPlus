#ifndef MESSAGEBUILDER_HPP
#define MESSAGEBUILDER_HPP

#include "MessageType.hpp"
#include <string>
#include <memory>
#include <unordered_map>

class MessageInterface;
class Message;
class MockMessage;
class User;
class Chatroom;

template<typename MessageObject>
class MessageBuilder {
    private:
        bool _success_bit;
        std::string _messageContents;
        std::string _to_user_name;
        int _to_user_id;
        std::string _from_user_name;
        int _from_user_id;
        int _to_chatroom_id;
        int _from_chatroom_id;
        MessageType _messageType;
        
        // Attributes from Message Class
        std::shared_ptr<User> _user;
        std::shared_ptr<Chatroom> _chatroom;
        std::shared_ptr<std::unordered_map<int, Chatroom>> _chatrooms;
        std::shared_ptr<std::unordered_map<int, std::string>> _userIDToUsername;

    public:
        MessageBuilder() : 
            _success_bit(true),
            _messageContents(""), 
            _to_user_name(""), 
            _to_user_id(-1), 
            _from_user_name(""), 
            _from_user_id(-1), 
            _to_chatroom_id(-1), 
            _from_chatroom_id(-1), 
            _messageType(MessageType::UNDEFINED),
            _user(nullptr),
            _chatroom(nullptr),
            _chatrooms(nullptr),
            _userIDToUsername(nullptr) {}
            
        MessageBuilder(MessageObject* message);

        inline void setSuccessBit(bool success_bit) {_success_bit = success_bit;}
        inline bool getSuccessBit() const {return _success_bit;}

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

        inline void setUser(std::shared_ptr<User> user) { _user = user; }
        inline std::shared_ptr<User> getUser() const { return _user; }
        
        inline void setChatroom(std::shared_ptr<Chatroom> chatroom) { _chatroom = chatroom; }
        inline std::shared_ptr<Chatroom> getChatroom() const { return _chatroom; }
        
        inline void setChatrooms(std::shared_ptr<std::unordered_map<int, Chatroom>> chatrooms) { _chatrooms = chatrooms; }
        inline std::shared_ptr<std::unordered_map<int, Chatroom>> getChatrooms() const { return _chatrooms; }
        
        inline void setUserIDToUsername(std::shared_ptr<std::unordered_map<int, std::string>> userIDToUsername) { _userIDToUsername = userIDToUsername; }
        inline std::shared_ptr<std::unordered_map<int, std::string>> getUserIDToUsername() const { return _userIDToUsername; }

        MessageObject buildMessage();
};

#endif