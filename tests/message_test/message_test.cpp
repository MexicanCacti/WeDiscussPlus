#include <gtest/gtest.h>
#include "shared_include/Message.hpp"
#include "shared_include/MessageBuilder.hpp"

bool successBit = true;
std::string messageContents = "Test Contents";
std::string toUsername = "Test ToUsername";
int toUserID = 99;
std::string fromUsername = "Test FromUsername";
int fromUserID = 100;
int toChatroomID = 0;
int fromChatroomID = 0;
MessageType messageType = MessageType::TEST;

void ExpectDefaultMessageState(const Message&);

void ExpectMessageState(const Message&, const bool, const std::string&, const std::string&, const int, const std::string&, const int, const int, const int, const MessageType);

void buildMessage(MessageBuilder<Message>&, const bool, const std::string&, const std::string&, const int, const std::string&, const int, const int, const int, const MessageType);

TEST(MessageTest, CreateMessageObject){
    ExpectDefaultMessageState(Message());
}

TEST(MessageTest, FullMessageBuilderProcess){
    MessageBuilder<Message> messageBuilder;
    ExpectDefaultMessageState(Message(&messageBuilder));
    buildMessage(messageBuilder, successBit, messageContents, toUsername, toUserID, fromUsername, fromUserID, toChatroomID, fromChatroomID, messageType);
    ExpectMessageState(Message(&messageBuilder), successBit, messageContents, toUsername, toUserID, fromUsername, fromUserID, toChatroomID, fromChatroomID, messageType);
}

TEST(MessageTest, SerializationDeserialization) {
    MessageBuilder<Message> builder;
    buildMessage(builder, successBit, messageContents, toUsername, toUserID, fromUsername, fromUserID, toChatroomID, fromChatroomID, messageType);
    Message original(&builder);
    
    std::vector<char> serialized = original.serialize();
    
    // Extract the size
    int messageSize;
    std::memcpy(&messageSize, serialized.data(), sizeof(int));
    
    // Get the message content
    std::vector<char> messageContent(serialized.begin() + sizeof(int), serialized.end());
    
    Message deserialized = Message::deserialize(messageContent);
    
    ExpectMessageState(deserialized, successBit, messageContents, toUsername, toUserID, fromUsername, fromUserID, toChatroomID, fromChatroomID, messageType);
}

TEST(MessageTest, MessageTypeToString) {
    EXPECT_EQ(Message::messageTypeToString(MessageType::CONNECT), "CONNECT");
    EXPECT_EQ(Message::messageTypeToString(MessageType::AUTHENTICATE), "AUTHENTICATE");
    EXPECT_EQ(Message::messageTypeToString(MessageType::LOGOUT), "LOGOUT");
    EXPECT_EQ(Message::messageTypeToString(MessageType::ADD_USER), "ADD_USER");
    EXPECT_EQ(Message::messageTypeToString(MessageType::CHANGE_USER_PASSWORD), "CHANGE_USER_PASSWORD");
    EXPECT_EQ(Message::messageTypeToString(MessageType::CHANGE_USER_NAME), "CHANGE_USER_NAME");
    EXPECT_EQ(Message::messageTypeToString(MessageType::DELETE_USER), "DELETE_USER");
    EXPECT_EQ(Message::messageTypeToString(MessageType::SEND_MESSAGE_TO_USER), "SEND_MESSAGE_TO_USER");
    EXPECT_EQ(Message::messageTypeToString(MessageType::CREATE_CHATROOM), "CREATE_CHATROOM");
    EXPECT_EQ(Message::messageTypeToString(MessageType::DELETE_CHATROOM), "DELETE_CHATROOM");
    EXPECT_EQ(Message::messageTypeToString(MessageType::ADD_USER_TO_CHATROOM), "ADD_USER_TO_CHATROOM");
    EXPECT_EQ(Message::messageTypeToString(MessageType::SEND_MESSAGE_TO_CHATROOM), "SEND_MESSAGE_TO_CHATROOM");
    EXPECT_EQ(Message::messageTypeToString(MessageType::REMOVE_USER_FROM_CHATROOM), "REMOVE_USER_FROM_CHATROOM");
    EXPECT_EQ(Message::messageTypeToString(MessageType::GET_USER_MESSAGES), "GET_USER_MESSAGES");
    EXPECT_EQ(Message::messageTypeToString(MessageType::GET_CHATROOM_MESSAGES), "GET_CHATROOM_MESSAGES");
    EXPECT_EQ(Message::messageTypeToString(MessageType::UNDEFINED), "UNDEFINED");
    EXPECT_EQ(Message::messageTypeToString(static_cast<MessageType>(999)), "UNKNOWN");
}

TEST(MessageTest, PartialMessageBuilder) {
    MessageBuilder<Message> builder;
    builder.setMessageContents(messageContents);
    builder.setMessageType(messageType);
    
    Message message(&builder);
    EXPECT_EQ(message.getMessageContents(), messageContents);
    EXPECT_EQ(message.getMessageType(), messageType);
    EXPECT_EQ(message.getToUserID(), -1);  // Should still be default
}

TEST(MessageTest, PrintMessage) {
    MessageBuilder<Message> builder;
    buildMessage(builder, successBit, messageContents, toUsername, toUserID, fromUsername, fromUserID, toChatroomID, fromChatroomID, messageType);
    Message message(&builder);
    
    
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    message.printMessage();
    std::cout.rdbuf(old);
    
    EXPECT_TRUE(buffer.str().find("Test Contents") != std::string::npos);
}

void ExpectDefaultMessageState(const Message& message){
    EXPECT_EQ(message.getSuccessBit(), true);
    EXPECT_EQ(message.getMessageContents(), "");
    EXPECT_EQ(message.getToUsername(), "");
    EXPECT_EQ(message.getToUserID(), -1);
    EXPECT_EQ(message.getFromUsername(), "");
    EXPECT_EQ(message.getFromUserID(), -1);
    EXPECT_EQ(message.getToChatroomID(), -1);
    EXPECT_EQ(message.getFromChatroomID(), -1);
    EXPECT_EQ(message.getMessageType(), MessageType::UNDEFINED);
}

void ExpectMessageState(const Message& message, const bool successBit, const std::string& messageContents, const std::string& toUsername, const int toUserID, const std::string& fromUsername, const int fromUserID, const int toChatroomID, const int fromChatroomID, const MessageType messageType){
    EXPECT_EQ(message.getSuccessBit(), successBit);
    EXPECT_EQ(message.getMessageContents(), messageContents);
    EXPECT_EQ(message.getToUsername(), toUsername);
    EXPECT_EQ(message.getToUserID(), toUserID);
    EXPECT_EQ(message.getFromUsername(), fromUsername);
    EXPECT_EQ(message.getFromUserID(), fromUserID);
    EXPECT_EQ(message.getToChatroomID(), toChatroomID);
    EXPECT_EQ(message.getFromChatroomID(), fromChatroomID);
    EXPECT_EQ(message.getMessageType(), messageType);
}

void buildMessage(MessageBuilder<Message>& messageBuilder, const bool successBit, const std::string& messageContents, const std::string& toUsername, const int toUserID, const std::string& fromUsername, const int fromUserID, const int toChatroomID, const int fromChatroomID, const MessageType messageType) {
    messageBuilder.setSuccessBit(successBit);
    messageBuilder.setMessageContents(messageContents);
    messageBuilder.setToUsername(toUsername);
    messageBuilder.setToUserID(toUserID);
    messageBuilder.setFromUsername(fromUsername);
    messageBuilder.setFromUserID(fromUserID);
    messageBuilder.setToChatroomID(toChatroomID);
    messageBuilder.setFromChatroomID(fromChatroomID);
    messageBuilder.setMessageType(messageType);
}