#include <gtest/gtest.h>
#include "shared_include/Message.hpp"
#include "shared_include/MessageBuilder.hpp"

std::string messageContents = "Test Contents";
std::string toUsername = "Test ToUsername";
int toUserID = 99;
std::string fromUsername = "Test FromUsername";
int fromUserID = 100;
int toChatroomID = 0;
int fromChatroomID = 0;
MessageType messageType = MessageType::TEST;

void ExpectDefaultMessageState(const Message&);

void ExpectMessageState(const Message&, const std::string&, const std::string&, const int, const std::string&, const int, const int, const int, const MessageType);

void buildMessage(MessageBuilder&, const std::string&, const std::string&, const int, const std::string&, const int, const int, const int, const MessageType);

TEST(MessageTest, CreateMessageObject){
    ExpectDefaultMessageState(Message());
}

TEST(MessageTest, FullMessageBuilderProcess){
    MessageBuilder messageBuilder = MessageBuilder();

    ExpectDefaultMessageState(messageBuilder.buildMessage());
    buildMessage(messageBuilder, messageContents, toUsername, toUserID, fromUsername, fromUserID, toChatroomID, fromChatroomID, messageType);
    ExpectMessageState(messageBuilder.buildMessage(), messageContents, toUsername, toUserID, fromUsername, fromUserID, toChatroomID, fromChatroomID, messageType);
}

void ExpectDefaultMessageState(const Message& message){
    EXPECT_EQ(message.getMessageContents(), "");
    EXPECT_EQ(message.getToUsername(), "");
    EXPECT_EQ(message.getToUserID(), -1);
    EXPECT_EQ(message.getFromUsername(), "");
    EXPECT_EQ(message.getFromUserID(), -1);
    EXPECT_EQ(message.getToChatroomID(), -1);
    EXPECT_EQ(message.getFromChatroomID(), -1);
    EXPECT_EQ(message.getMessageType(), MessageType::TEST);
}

void ExpectMessageState(const Message& message, const std::string& messageContents, const std::string& toUsername, const int toUserID, const std::string& fromUsername, const int fromUserID, const int toChatroomID, const int fromChatroomID, const MessageType messageType){
    EXPECT_EQ(message.getMessageContents(), messageContents);
    EXPECT_EQ(message.getToUsername(), toUsername);
    EXPECT_EQ(message.getToUserID(), toUserID);
    EXPECT_EQ(message.getFromUsername(), fromUsername);
    EXPECT_EQ(message.getFromUserID(), fromUserID);
    EXPECT_EQ(message.getToChatroomID(), toChatroomID);
    EXPECT_EQ(message.getFromChatroomID(), fromChatroomID);
    EXPECT_EQ(message.getMessageType(), messageType);
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