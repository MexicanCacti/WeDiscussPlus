#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include "MessageInterface.hpp"
#include "MessageBuilder.hpp"
#include <iostream>
#include <memory>
#include <unordered_map>

class User;
class Chatroom;

class Message : public MessageInterface {
    private:
        std::shared_ptr<User> _user;
        std::shared_ptr<Chatroom> _chatroom;
        std::shared_ptr<std::unordered_map<int, Chatroom>> _chatrooms;
        std::shared_ptr<std::unordered_map<int, std::string>> _userIDToUsername;
        
    public:
        Message() : MessageInterface() {
            _user = nullptr;
            _chatroom = nullptr;
            _chatrooms = nullptr;
            _userIDToUsername = nullptr;
        };

        Message(MessageBuilder<Message>* messageBuilder);

        std::vector<char> serialize() const override;

        static Message deserialize(const std::vector<char>& data);
        
        void printMessage() const override;
};

#endif