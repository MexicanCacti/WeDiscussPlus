#include <gtest/gtest.h>
#include "server_include/Server.hpp"
#include "server_include/UserManager.hpp"
#include "server_include/ChatroomManager.hpp"
#include "server_include/LogManager.hpp"
#include "shared_include/Message.hpp"
#include "shared_include/MessageBuilder.hpp"
#include <memory>
#include <thread>
#include <iostream>
#include <chrono>
#include <asio.hpp>
#include <vector>

using namespace std::chrono_literals;
using asio::ip::tcp;

asio::io_context context;
tcp::resolver resolver(context);
auto endpoints = resolver.resolve("127.0.0.1", "3333");

std::string messageContents = "Test Contents";
std::string toUsername = "Test ToUsername";
int toUserID = 99;
std::string fromUsername = "Test FromUsername";
int fromUserID = 100;
int toChatroomID = 0;
int fromChatroomID = 0;

void buildMessage(MessageBuilder&, const std::string&, const std::string&, const int, const std::string&, const int, const int, const int, const MessageType);
Message readMessageFromSocket(tcp::socket& socket);

class RoutingTest : public ::testing::Test {
protected:
    void SetUp() override {
        _server = std::make_unique<Server>(3333);
        std::cout << "Starting Server..." << std::endl;
        std::thread([this](){_server->startServer();}).detach();
        std::this_thread::sleep_for(100ms);
        std::cout << "Server started" << std::endl;
    }

    void TearDown() override {
        std::cout << "Stopping Server..." << std::endl;
        _server->stopServer();
        std::this_thread::sleep_for(100ms);
        std::cout << "Server Stopped" << std::endl;
    }

    void establishConnection(tcp::socket& sendSocket, tcp::socket& recvSocket) {
        // Connect send socket
        asio::connect(sendSocket, endpoints);
        
        // Send initial SEND message
        MessageBuilder sendBuilder;
        buildMessage(sendBuilder, "SEND", toUsername, toUserID, fromUsername, fromUserID, toChatroomID, fromChatroomID, MessageType::SEND);
        Message sendMessage(&sendBuilder);
        std::vector<char> sendData = sendMessage.serialize();
        asio::write(sendSocket, asio::buffer(sendData));

        // Connect receive socket
        asio::connect(recvSocket, endpoints);
        
        // Send initial RECV message
        MessageBuilder recvBuilder;
        buildMessage(recvBuilder, "RECV", toUsername, toUserID, fromUsername, fromUserID, toChatroomID, fromChatroomID, MessageType::RECV);
        Message recvMessage(&recvBuilder);
        std::vector<char> recvData = recvMessage.serialize();
        asio::write(recvSocket, asio::buffer(recvData));

        // Wait for ACK
        Message ackMessage = readMessageFromSocket(recvSocket);
        EXPECT_EQ(ackMessage.getMessageType(), MessageType::ACK);
    }

private:
    std::unique_ptr<Server> _server;
};

Message readMessageFromSocket(tcp::socket& socket) {
    int msgSize = 0;
    asio::read(socket, asio::buffer(&msgSize, sizeof(int)));
    if(msgSize <= 0) {
        throw std::runtime_error("Invalid message size received: " + std::to_string(msgSize));
    }
    std::vector<char> buffer(msgSize);
    asio::read(socket, asio::buffer(buffer.data(), msgSize));
    return Message::deserialize(buffer);
}

TEST_F(RoutingTest, UserManagerRouting) {
    tcp::socket sendSocket(context);
    tcp::socket recvSocket(context);
    establishConnection(sendSocket, recvSocket);

    std::vector<std::pair<MessageType, std::string>> userManagerTypes = {
        {MessageType::LOGOUT, "logoutUser"},
        {MessageType::ADD_USER, "addUser"},
        {MessageType::CHANGE_USER_PASSWORD, "changeUserPassword"},
        {MessageType::CHANGE_USER_NAME, "changeUserName"},
        {MessageType::DELETE_USER, "deleteUser"},
        {MessageType::SEND_MESSAGE_TO_USER, "sendMessageToUser"},
        {MessageType::SEND, "authUser"}
    };

    for(const auto& [messageType, expectedFunction] : userManagerTypes) {
        MessageBuilder builder;
        buildMessage(builder, messageContents, toUsername, toUserID, fromUsername, fromUserID, toChatroomID, fromChatroomID, messageType);
        Message message(&builder);
        std::vector<char> data = message.serialize();
        asio::write(sendSocket, asio::buffer(data));

        // Read response
        Message response = readMessageFromSocket(recvSocket);
        EXPECT_EQ(response.getMessageContents(), expectedFunction);
    }
}

TEST_F(RoutingTest, ChatroomManagerRouting) {
    tcp::socket sendSocket(context);
    tcp::socket recvSocket(context);
    establishConnection(sendSocket, recvSocket);

    std::vector<std::pair<MessageType, std::string>> chatroomManagerTypes = {
        {MessageType::CREATE_CHATROOM, "createChatroom"},
        {MessageType::DELETE_CHATROOM, "deleteChatroom"},
        {MessageType::ADD_USER_TO_CHATROOM, "addUserToChatroom"},
        {MessageType::SEND_MESSAGE_TO_CHATROOM, "sendMessageToChatroom"},
        {MessageType::REMOVE_USER_FROM_CHATROOM, "removeUserFromChatroom"}
    };

    for(const auto& [messageType, expectedFunction] : chatroomManagerTypes) {
        MessageBuilder builder;
        buildMessage(builder, messageContents, toUsername, toUserID, fromUsername, fromUserID, toChatroomID, fromChatroomID, messageType);
        Message message(&builder);
        std::vector<char> data = message.serialize();
        asio::write(sendSocket, asio::buffer(data));

        // Read response
        Message response = readMessageFromSocket(recvSocket);
        EXPECT_EQ(response.getMessageContents(), expectedFunction);
    }
}

TEST_F(RoutingTest, LogManagerRouting) {
    tcp::socket sendSocket(context);
    tcp::socket recvSocket(context);
    establishConnection(sendSocket, recvSocket);

    std::vector<std::pair<MessageType, std::string>> logManagerTypes = {
        {MessageType::GET_USER_MESSAGES, "getUserMessages"},
        {MessageType::GET_CHATROOM_MESSAGES, "getChatroomMessages"}
    };

    for(const auto& [messageType, expectedFunction] : logManagerTypes) {
        MessageBuilder builder;
        buildMessage(builder, messageContents, toUsername, toUserID, fromUsername, fromUserID, toChatroomID, fromChatroomID, messageType);
        Message message(&builder);
        std::vector<char> data = message.serialize();
        asio::write(sendSocket, asio::buffer(data));

        // Read response
        Message response = readMessageFromSocket(recvSocket);
        EXPECT_EQ(response.getMessageContents(), expectedFunction);
    }
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