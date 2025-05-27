#include <gtest/gtest.h>
#include "MessageInterface.hpp"
#include "MessageBuilder.hpp"
#include "MessageFactory.hpp"
#include "ConnectMessage.hpp"
#include "AuthMessage.hpp"
#include "UserMessage.hpp"
#include "ChatroomMessage.hpp"
#include "AuthResponseMessage.hpp"
#include <sstream>

const bool successBit = true;
const std::string messageContents = "Test Contents";
const std::string toUsername = "Test ToUsername";
const int toUserID = 99;
const std::string fromUsername = "Test FromUsername";
const int fromUserID = 100;
const int toChatroomID = 1;

const std::unordered_map<int, std::string> testUserMap = {
    {1, "user1"},
    {2, "user2"},
    {3, "user3"}
};

std::vector<ChatroomData> testChatrooms;
std::vector<std::shared_ptr<MessageInterface>> testInbox;

void initializeTestData() {
    ChatroomData chatroom1;
    chatroom1.id = 1;
    chatroom1.memberIds = {1, 2, 3};

    MessageBuilder builder;

    builder.setMessageType(MessageType::SEND_MESSAGE_TO_CHATROOM);
    builder.setMessageContents("Chatroom1 Message 1");
    builder.setFromUser("user1", 1);
    builder.setToChatroom(1);
    auto msg1 = builder.build();
    builder.setMessageContents("Chatroom1 Message 2");
    builder.setFromUser("user2", 2);
    auto msg2 = builder.build();
    chatroom1.messages = {msg1, msg2};
    testChatrooms.push_back(chatroom1);

    builder.setMessageType(MessageType::SEND_MESSAGE_TO_USER);
    builder.setMessageContents("Inbox Message 1");
    builder.setFromUser("user2", 2);
    builder.setToUser("user1", 1);
    auto inboxMsg1 = builder.build();
    
    builder.setMessageContents("Inbox Message 2");
    builder.setFromUser("user3", 3);
    auto inboxMsg2 = builder.build();
    
    testInbox = {inboxMsg1, inboxMsg2};
}

void compareUserMap(const std::unordered_map<int, std::string>& userMap, const std::unordered_map<int, std::string>& expectedUserMap) {
    EXPECT_EQ(userMap.size(), expectedUserMap.size());
    for (const auto& [id, name] : userMap) {
        EXPECT_EQ(name, expectedUserMap.at(id));
    }
}

void compareChatrooms(const std::vector<ChatroomData>& chatrooms, const std::vector<ChatroomData>& expectedChatrooms) {
    EXPECT_EQ(chatrooms.size(), expectedChatrooms.size());
    for (size_t i = 0; i < chatrooms.size(); ++i) {
        EXPECT_EQ(chatrooms[i].id, expectedChatrooms[i].id);
        EXPECT_EQ(chatrooms[i].memberIds, expectedChatrooms[i].memberIds);
        EXPECT_EQ(chatrooms[i].messages.size(), expectedChatrooms[i].messages.size());
        
        for (size_t j = 0; j < chatrooms[i].messages.size(); ++j) {
            const auto& msg = chatrooms[i].messages[j];
            const auto& expectedMsg = expectedChatrooms[i].messages[j];
            EXPECT_EQ(msg->getMessageType(), expectedMsg->getMessageType());
            EXPECT_EQ(msg->getMessageContents(), expectedMsg->getMessageContents());
            EXPECT_EQ(msg->getFromUsername(), expectedMsg->getFromUsername());
            EXPECT_EQ(msg->getFromUserID(), expectedMsg->getFromUserID());
            EXPECT_EQ(msg->getToChatroomID(), expectedMsg->getToChatroomID());
        }
    }
}

void compareInbox(const std::vector<std::shared_ptr<MessageInterface>>& inbox, const std::vector<std::shared_ptr<MessageInterface>>& expectedInbox) {
    EXPECT_EQ(inbox.size(), expectedInbox.size());
    for (size_t i = 0; i < inbox.size(); ++i) {
        EXPECT_EQ(inbox[i]->getMessageType(), expectedInbox[i]->getMessageType());
        EXPECT_EQ(inbox[i]->getMessageContents(), expectedInbox[i]->getMessageContents());
        EXPECT_EQ(inbox[i]->getToUsername(), expectedInbox[i]->getToUsername());
        EXPECT_EQ(inbox[i]->getToUserID(), expectedInbox[i]->getToUserID());
        EXPECT_EQ(inbox[i]->getFromUsername(), expectedInbox[i]->getFromUsername());
        EXPECT_EQ(inbox[i]->getFromUserID(), expectedInbox[i]->getFromUserID());
    }
}

void ExpectDefaultMessageState(const std::shared_ptr<MessageInterface>& message) {
    EXPECT_EQ(message->getSuccessBit(), true);
    EXPECT_EQ(message->getMessageContents(), "");
    EXPECT_EQ(message->getToUsername(), "");
    EXPECT_EQ(message->getToUserID(), -1);
    EXPECT_EQ(message->getFromUsername(), "");
    EXPECT_EQ(message->getFromUserID(), -1);
    EXPECT_EQ(message->getToChatroomID(), -1);
}

void ExpectMessageState(const std::shared_ptr<MessageInterface>& message,
        const bool successBit,
        const std::string& messageContents,
        const std::string& toUsername,
        const int toUserID,
        const std::string& fromUsername,
        const int fromUserID,
        const int toChatroomID)
    {
    // Data Checks
    EXPECT_EQ(message->getSuccessBit(), successBit);
    
    if (dynamic_cast<AuthResponseMessage*>(message.get()) != nullptr) {
        const auto* authResponse = dynamic_cast<AuthResponseMessage*>(message.get());
        EXPECT_EQ(authResponse->getToUsername(), toUsername);
        EXPECT_EQ(authResponse->getToUserID(), toUserID);
        compareUserMap(authResponse->getUserMap(), testUserMap);
        compareChatrooms(authResponse->getChatrooms(), testChatrooms);
        compareInbox(authResponse->getInbox(), testInbox);
    }
    else if (dynamic_cast<UserMessage*>(message.get()) != nullptr) {
        EXPECT_EQ(message->getToUsername(), toUsername);
        EXPECT_EQ(message->getToUserID(), toUserID);
        EXPECT_EQ(message->getFromUsername(), fromUsername);
        EXPECT_EQ(message->getFromUserID(), fromUserID);
        EXPECT_EQ(message->getMessageContents(), messageContents);
    }
    else if (dynamic_cast<ChatroomMessage*>(message.get()) != nullptr) {
        EXPECT_EQ(message->getToChatroomID(), toChatroomID);
        EXPECT_EQ(message->getFromUsername(), fromUsername);
        EXPECT_EQ(message->getFromUserID(), fromUserID);
        EXPECT_EQ(message->getMessageContents(), messageContents);
    }
    
    
    // Message Type Checks
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
    else if (dynamic_cast<AuthResponseMessage*>(message.get()) != nullptr) {
        EXPECT_EQ(message->getMessageType(), MessageType::AUTH_RESPONSE);
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
                      fromUsername, fromUserID, -1);
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
                      fromUsername, fromUserID, -1);
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
                      fromUsername, fromUserID, -1);
}

TEST(MessageTest, CreateChatroomMessage) {
    MessageBuilder builder;
    builder.setMessageType(MessageType::SEND_MESSAGE_TO_CHATROOM);
    builder.setSuccessBit(successBit);
    builder.setMessageContents(messageContents);
    builder.setFromUser(fromUsername, fromUserID);
    builder.setToChatroom(toChatroomID);
    auto message = builder.build();
    
    ExpectMessageState(message, successBit, messageContents, toUsername, toUserID, 
                      fromUsername, fromUserID, toChatroomID);
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
    auto deserialized = MessageFactory::deserialize(serialized, offset);
    
    ExpectMessageState(deserialized, successBit, messageContents, toUsername, toUserID, 
                      fromUsername, fromUserID, -1);
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

TEST(MessageTest, AuthResponseMessage) {
    initializeTestData();  // Initialize test data before running the test
    
    MessageBuilder builder;
    builder.setMessageType(MessageType::AUTH_RESPONSE);
    builder.setSuccessBit(successBit);
    builder.setMessageContents(messageContents);
    builder.setToUser(toUsername, toUserID);
    builder.setUserMap(testUserMap);
    builder.setChatrooms(testChatrooms);
    builder.setInbox(testInbox);
    auto authResponseMessage = builder.build();

    ExpectMessageState(authResponseMessage, successBit, messageContents, toUsername, toUserID, 
                      fromUsername, fromUserID, toChatroomID);
    
    std::vector<char> serialized = authResponseMessage->serialize();
    size_t offset = 0;
    auto deserialized = MessageFactory::deserialize(serialized, offset);

    ExpectMessageState(deserialized, successBit, messageContents, toUsername, toUserID, 
                      fromUsername, fromUserID, toChatroomID);

    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    dynamic_cast<AuthResponseMessage*>(deserialized.get())->printMessage();
    std::cout.rdbuf(old);
    
    EXPECT_TRUE(buffer.str().find("Authentication Response Message") != std::string::npos);
    EXPECT_TRUE(buffer.str().find("Success: Yes") != std::string::npos);
    EXPECT_TRUE(buffer.str().find("User Map Size: 3") != std::string::npos);
    EXPECT_TRUE(buffer.str().find("Chatrooms: 1") != std::string::npos);
    EXPECT_TRUE(buffer.str().find("Inbox Messages: 2") != std::string::npos);
}