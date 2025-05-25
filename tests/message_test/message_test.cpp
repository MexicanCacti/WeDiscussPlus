#include <gtest/gtest.h>
#include "MessageInterface.hpp"
#include "MessageBuilder.hpp"
#include "MessageFactory.hpp"
#include "ConnectMessage.hpp"
#include "AuthMessage.hpp"
#include "UserMessage.hpp"
#include "ChatroomMessage.hpp"
#include <sstream>

const bool successBit = true;
const std::string messageContents = "Test Contents";
const std::string toUsername = "Test ToUsername";
const int toUserID = 99;
const std::string fromUsername = "Test FromUsername";
const int fromUserID = 100;
const int toChatroomID = 1;
const int fromChatroomID = 2;

void ExpectDefaultMessageState(const std::shared_ptr<MessageInterface>& message) {
    EXPECT_EQ(message->getSuccessBit(), true);
    EXPECT_EQ(message->getMessageContents(), "");
    EXPECT_EQ(message->getToUsername(), "");
    EXPECT_EQ(message->getToUserID(), -1);
    EXPECT_EQ(message->getFromUsername(), "");
    EXPECT_EQ(message->getFromUserID(), -1);
    EXPECT_EQ(message->getToChatroomID(), -1);
    EXPECT_EQ(message->getFromChatroomID(), -1);
}

void ExpectMessageState(const std::shared_ptr<MessageInterface>& message,
                       const bool successBit,
                       const std::string& messageContents,
                       const std::string& toUsername,
                       const int toUserID,
                       const std::string& fromUsername,
                       const int fromUserID,
                       const int toChatroomID,
                       const int fromChatroomID) {
    EXPECT_EQ(message->getSuccessBit(), successBit);
    EXPECT_EQ(message->getMessageContents(), messageContents);
    if (dynamic_cast<UserMessage*>(message.get()) != nullptr) {
        EXPECT_EQ(message->getToUsername(), toUsername);
        EXPECT_EQ(message->getToUserID(), toUserID);
    }
    else if (dynamic_cast<ChatroomMessage*>(message.get()) != nullptr) {
        EXPECT_EQ(message->getToChatroomID(), toChatroomID);
        EXPECT_EQ(message->getFromChatroomID(), fromChatroomID);
    }
    EXPECT_EQ(message->getFromUsername(), fromUsername);
    EXPECT_EQ(message->getFromUserID(), fromUserID);
    
    // Check message type and validate specific fields
    if (dynamic_cast<ConnectMessage*>(message.get()) != nullptr) {
        EXPECT_EQ(message->getMessageType(), MessageType::CONNECT);
    }
    else if (dynamic_cast<AuthMessage*>(message.get()) != nullptr) {
        EXPECT_EQ(message->getMessageType(), MessageType::AUTHENTICATE);
    }
    else if (dynamic_cast<UserMessage*>(message.get()) != nullptr) {
        EXPECT_EQ(message->getMessageType(), MessageType::SEND_MESSAGE_TO_USER);
    }
    else if (dynamic_cast<ChatroomMessage*>(message.get()) != nullptr) {
        EXPECT_EQ(message->getMessageType(), MessageType::SEND_MESSAGE_TO_CHATROOM);
    }
    else {
        FAIL() << "Unknown message type";
    }
}

TEST(MessageTest, CreateConnectMessage) {
    MessageBuilder builder;
    builder.setMessageType(MessageType::CONNECT);
    builder.setSuccessBit(successBit);
    builder.setMessageContents(messageContents);
    builder.setToUser(toUsername, toUserID);
    builder.setFromUser(fromUsername, fromUserID);
    auto message = builder.build();
    
    ExpectMessageState(message, successBit, messageContents, toUsername, toUserID, 
                      fromUsername, fromUserID, -1, -1);
}

TEST(MessageTest, CreateAuthMessage) {
    MessageBuilder builder;
    builder.setMessageType(MessageType::AUTHENTICATE);
    builder.setSuccessBit(successBit);
    builder.setMessageContents(messageContents);
    builder.setToUser(toUsername, toUserID);
    builder.setFromUser(fromUsername, fromUserID);
    auto message = builder.build();
    
    ExpectMessageState(message, successBit, messageContents, toUsername, toUserID, 
                      fromUsername, fromUserID, -1, -1);
}

TEST(MessageTest, CreateUserMessage) {
    MessageBuilder builder;
    builder.setMessageType(MessageType::SEND_MESSAGE_TO_USER);
    builder.setSuccessBit(successBit);
    builder.setMessageContents(messageContents);
    builder.setToUser(toUsername, toUserID);
    builder.setFromUser(fromUsername, fromUserID);
    auto message = builder.build();
    
    ExpectMessageState(message, successBit, messageContents, toUsername, toUserID, 
                      fromUsername, fromUserID, -1, -1);
}

TEST(MessageTest, CreateChatroomMessage) {
    MessageBuilder builder;
    builder.setMessageType(MessageType::SEND_MESSAGE_TO_CHATROOM);
    builder.setSuccessBit(successBit);
    builder.setMessageContents(messageContents);
    builder.setFromUser(fromUsername, fromUserID);
    builder.setToChatroom(toChatroomID);
    builder.setFromChatroom(fromChatroomID);
    auto message = builder.build();
    
    ExpectMessageState(message, successBit, messageContents, toUsername, toUserID, 
                      fromUsername, fromUserID, toChatroomID, fromChatroomID);
}

TEST(MessageTest, SerializationDeserialization) {
    MessageBuilder builder;
    builder.setMessageType(MessageType::SEND_MESSAGE_TO_USER);
    builder.setSuccessBit(successBit);
    builder.setMessageContents(messageContents);
    builder.setToUser(toUsername, toUserID);
    builder.setFromUser(fromUsername, fromUserID);
    auto original = builder.build();
    
    std::vector<char> serialized = original->serialize();
    size_t offset = 0;
    auto deserialized = MessageFactory::deserialize(serialized);
    
    ExpectMessageState(deserialized, successBit, messageContents, toUsername, toUserID, 
                      fromUsername, fromUserID, -1, -1);
}

TEST(MessageTest, MessageTypeToString) {
    EXPECT_EQ(MessageInterface::messageTypeToString(MessageType::CONNECT), "CONNECT");
    EXPECT_EQ(MessageInterface::messageTypeToString(MessageType::AUTHENTICATE), "AUTHENTICATE");
    EXPECT_EQ(MessageInterface::messageTypeToString(MessageType::SEND_MESSAGE_TO_USER), "SEND_MESSAGE_TO_USER");
    EXPECT_EQ(MessageInterface::messageTypeToString(MessageType::SEND_MESSAGE_TO_CHATROOM), "SEND_MESSAGE_TO_CHATROOM");
    EXPECT_EQ(MessageInterface::messageTypeToString(MessageType::UNDEFINED), "UNDEFINED");
}

TEST(MessageTest, PrintMessage) {
    MessageBuilder builder;
    builder.setMessageType(MessageType::SEND_MESSAGE_TO_USER);
    builder.setSuccessBit(successBit);
    builder.setMessageContents(messageContents);
    builder.setToUser(toUsername, toUserID);
    builder.setFromUser(fromUsername, fromUserID);
    auto message = builder.build();
    
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    message->printMessage();
    std::cout.rdbuf(old);
    
    EXPECT_TRUE(buffer.str().find(messageContents) != std::string::npos);
    EXPECT_TRUE(buffer.str().find(toUsername) != std::string::npos);
    EXPECT_TRUE(buffer.str().find(fromUsername) != std::string::npos);
}