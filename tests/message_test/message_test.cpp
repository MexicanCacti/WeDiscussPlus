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
    MessageBuilder messageBuilder;
    ExpectDefaultMessageState(Message(&messageBuilder));
    buildMessage(messageBuilder, messageContents, toUsername, toUserID, fromUsername, fromUserID, toChatroomID, fromChatroomID, messageType);
    ExpectMessageState(Message(&messageBuilder), messageContents, toUsername, toUserID, fromUsername, fromUserID, toChatroomID, fromChatroomID, messageType);
}

TEST(MessageTest, SerializationDeserialization) {
    MessageBuilder builder;
    buildMessage(builder, messageContents, toUsername, toUserID, fromUsername, fromUserID, toChatroomID, fromChatroomID, messageType);
    Message original(&builder);
    
    std::vector<char> serialized = original.serialize();
    
    // Extract the size
    int messageSize;
    std::memcpy(&messageSize, serialized.data(), sizeof(int));
    
    // Get the message content
    std::vector<char> messageContent(serialized.begin() + sizeof(int), serialized.end());
    
    Message deserialized = Message::deserialize(messageContent);
    
    ExpectMessageState(deserialized, messageContents, toUsername, toUserID, fromUsername, fromUserID, toChatroomID, fromChatroomID, messageType);
}

TEST(MessageTest, MessageTypeToString) {
    EXPECT_EQ(Message::messageTypeToString(MessageType::SEND), "SEND");
    EXPECT_EQ(Message::messageTypeToString(MessageType::RECV), "RECV");
    EXPECT_EQ(Message::messageTypeToString(MessageType::UNDEFINED), "UNDEFINED");
    EXPECT_EQ(Message::messageTypeToString(static_cast<MessageType>(999)), "UNKNOWN");
}

TEST(MessageTest, PartialMessageBuilder) {
    MessageBuilder builder;
    builder.setMessageContents(messageContents);
    builder.setMessageType(messageType);
    
    Message message(&builder);
    EXPECT_EQ(message.getMessageContents(), messageContents);
    EXPECT_EQ(message.getMessageType(), messageType);
    EXPECT_EQ(message.getToUserID(), -1);  // Should still be default
}

TEST(MessageTest, PrintMessage) {
    MessageBuilder builder;
    buildMessage(builder, messageContents, toUsername, toUserID, fromUsername, fromUserID, toChatroomID, fromChatroomID, messageType);
    Message message(&builder);
    
    
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    message.printMessage();
    std::cout.rdbuf(old);
    
    EXPECT_TRUE(buffer.str().find("Test Contents") != std::string::npos);
}

void ExpectDefaultMessageState(const Message& message){
    EXPECT_EQ(message.getMessageContents(), "");
    EXPECT_EQ(message.getToUsername(), "");
    EXPECT_EQ(message.getToUserID(), -1);
    EXPECT_EQ(message.getFromUsername(), "");
    EXPECT_EQ(message.getFromUserID(), -1);
    EXPECT_EQ(message.getToChatroomID(), -1);
    EXPECT_EQ(message.getFromChatroomID(), -1);
    EXPECT_EQ(message.getMessageType(), MessageType::UNDEFINED);
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