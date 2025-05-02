#include <gtest/gtest.h>
#include "shared_include/Message.hpp"
#include "shared_include/MessageBuilder.hpp"
#include "server_include/Server.hpp"

void buildMessage(MessageBuilder&, const std::string&, const std::string&, const int, const std::string&, const int, const int, const int, const MessageType);

std::string messageContents = "Test Contents";
std::string toUsername = "Test ToUsername";
int toUserID = 99;
std::string fromUsername = "Test FromUsername";
int fromUserID = 100;
int toChatroomID = 0;
int fromChatroomID = 0;
MessageType messageType = MessageType::TEST;

MessageBuilder messageBuilder = MessageBuilder();

TEST(UserBalancerTest, addMessageToQueue){
    auto server = std::make_shared<Server>();
    server->startServer(1655);
    buildMessage(messageBuilder, messageContents, toUsername, toUserID, fromUsername, fromUserID, toChatroomID, fromChatroomID, messageType);
    Message message = messageBuilder.buildMessage();
    server->addMessageToUserBalancer(message);
}

TEST(ChatroomBalancerTest, addMessageToQueue){
    auto server = std::make_shared<Server>();
    server->startServer(1655);
    buildMessage(messageBuilder, messageContents, toUsername, toUserID, fromUsername, fromUserID, toChatroomID, fromChatroomID, messageType);
    Message message = messageBuilder.buildMessage();
    server->addMessageToChatroomBalancer(message);
}

void buildMessage(MessageBuilder& messageBuilder, const std::string& messageContents, const std::string& toUsername, const int toUserID, const std::string& fromUsername, const int fromUserID, const int toChatroomID, const int fromChatroomID, const MessageType messageType) {
    messageBuilder.setMessageContents(messageContents);
    messageBuilder.setToUsername(toUsername);
    messageBuilder.setToUserID(toUserID);
    messageBuilder.setFromUsername(fromUsername);
    messageBuilder.setFromUserID(fromUserID);
    messageBuilder.setToChatroomID(toChatroomID);
    messageBuilder.setFromChatroomID(fromChatroomID);
    messageBuilder.setMessageType(messageType);
}