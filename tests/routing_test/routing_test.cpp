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
#include <future>

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

void buildMessage(MessageBuilder<MockMessage>&, const std::string&, const std::string&, const int, const std::string&, const int, const int, const int, const MessageType);
MockMessage readMessageFromSocket(tcp::socket& socket);

class RoutingTest : public ::testing::Test {
protected:
    void SetUp() override {
        try {
            _server = std::make_unique<Server<MockMessage>>(3333);
            
            std::promise<void> serverStarted;
            std::future<void> serverStartedFuture = serverStarted.get_future();
            
            std::cout << "Creating server thread..." << std::endl;
            std::thread serverThread([this, &serverStarted](){
                try {
                    std::cout << "Server thread starting..." << std::endl;
                    _server->startServer();
                    std::cout << "Server started successfully in thread" << std::endl;
                    serverStarted.set_value();
                } catch (const std::exception& e) {
                    std::cerr << "Server thread exception: " << e.what() << std::endl;
                    serverStarted.set_exception(std::current_exception());
                }
            });

            std::cout << "Waiting for server to start..." << std::endl;
            try {
                auto status = serverStartedFuture.wait_for(2s);
                if (status == std::future_status::timeout) {
                    throw std::runtime_error("Server failed to start within timeout period");
                }
                std::cout << "Server started successfully" << std::endl;
            } catch (const std::exception& e) {
                std::cerr << "Failed to start server: " << e.what() << std::endl;
                throw;
            }

            serverThread.detach();
        } catch (const std::exception& e) {
            std::cerr << "Error in SetUp: " << e.what() << std::endl;
            throw;
        }
    }

    void TearDown() override {
        std::cout << "Stopping Server..." << std::endl;
        _server->stopServer();
        std::cout << "Server Stopped" << std::endl;
    }

    void establishConnection(tcp::socket& sendSocket, tcp::socket& recvSocket) {
        std::cout << "Starting connection establishment..." << std::endl;
        try {
            std::cout << "Connecting send socket..." << std::endl;
            sendSocket.connect(tcp::endpoint(asio::ip::address::from_string("127.0.0.1"), 3333));
            std::cout << "Send socket connected successfully" << std::endl;

            MessageBuilder<MockMessage> sendBuilder;
            buildMessage(sendBuilder, "SEND", toUsername, toUserID, fromUsername, fromUserID, toChatroomID, fromChatroomID, MessageType::SEND);
            MockMessage sendMessage(&sendBuilder);
            std::vector<char> sendData = sendMessage.serialize();
            asio::write(sendSocket, asio::buffer(sendData));
            std::cout << "SEND message sent successfully" << std::endl;

            MockMessage authResponse = readMessageFromSocket(sendSocket);

            EXPECT_EQ(authResponse.getMessageContents(), "authUser");
            std::cout << "Auth response verified successfully" << std::endl;

            recvSocket.connect(tcp::endpoint(asio::ip::address::from_string("127.0.0.1"), 3333));
            std::cout << "Receive socket connected successfully" << std::endl;

            MessageBuilder<MockMessage> recvBuilder;
            buildMessage(recvBuilder, "RECV", toUsername, toUserID, fromUsername, fromUserID, toChatroomID, fromChatroomID, MessageType::RECV);
            MockMessage recvMessage(&recvBuilder);
            std::vector<char> recvData = recvMessage.serialize();
            asio::write(recvSocket, asio::buffer(recvData));
            std::cout << "RECV message sent successfully" << std::endl;

            MockMessage ackMessage = readMessageFromSocket(recvSocket);
            EXPECT_EQ(ackMessage.getMessageType(), MessageType::ACK);
            std::cout << "ACK verified successfully" << std::endl;

            std::cout << "Connection establishment completed" << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Error in establishConnection: " << e.what() << std::endl;
            throw;
        }
    }

private:
    std::unique_ptr<Server<MockMessage>> _server;
};

TEST_F(RoutingTest, BasicConnection) {
    try {
        tcp::socket sendSocket(context);
        tcp::socket recvSocket(context);
        std::cout << "Sockets created successfully" << std::endl;
        
        establishConnection(sendSocket, recvSocket);
        
        std::cout << "Basic connection test completed successfully" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error in BasicConnection test: " << e.what() << std::endl;
        throw;
    }
}

TEST_F(RoutingTest, UserManagerRouting) {
    std::cout << "Starting UserManager routing test" << std::endl;
    try {
        tcp::socket sendSocket(context);
        tcp::socket recvSocket(context);
        std::cout << "Sockets created, establishing connection..." << std::endl;
        establishConnection(sendSocket, recvSocket);
        std::cout << "Connection established successfully" << std::endl;

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
            std::cout << "\nTesting message type: " << MockMessage::messageTypeToString(messageType) << std::endl;
            
            MessageBuilder<MockMessage> builder;
            buildMessage(builder, messageContents, toUsername, toUserID, fromUsername, fromUserID, toChatroomID, fromChatroomID, messageType);
            MockMessage message(&builder);
            std::vector<char> data = message.serialize();
            
            try {
                asio::write(sendSocket, asio::buffer(data));
            } catch (const std::exception& e) {
                std::cerr << "Failed to send message: " << e.what() << std::endl;
                throw;
            }

            try {
                MockMessage response = readMessageFromSocket(recvSocket);
                std::cout << "Received response: " << response.getMessageContents() << std::endl;
                EXPECT_EQ(response.getMessageContents(), expectedFunction);
            } catch (const std::exception& e) {
                std::cerr << "Failed to read response: " << e.what() << std::endl;
                throw;
            }
            
        }
        std::cout << "UserManager routing test completed successfully" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error in UserManagerRouting test: " << e.what() << std::endl;
        throw;
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
        MessageBuilder<MockMessage> builder;
        buildMessage(builder, messageContents, toUsername, toUserID, fromUsername, fromUserID, toChatroomID, fromChatroomID, messageType);
        MockMessage message(&builder);
        std::vector<char> data = message.serialize();
        asio::write(sendSocket, asio::buffer(data));

        MockMessage response = readMessageFromSocket(recvSocket);
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
        MessageBuilder<MockMessage> builder;
        buildMessage(builder, messageContents, toUsername, toUserID, fromUsername, fromUserID, toChatroomID, fromChatroomID, messageType);
        MockMessage message(&builder);
        std::vector<char> data = message.serialize();
        asio::write(sendSocket, asio::buffer(data));

        MockMessage response = readMessageFromSocket(recvSocket);
        EXPECT_EQ(response.getMessageContents(), expectedFunction);
    }
}

MockMessage readMessageFromSocket(tcp::socket& socket) {
    int msgSize = 0;
    try {
        
        asio::read(socket, asio::buffer(&msgSize, sizeof(int)));
        std::cout << "Message size received: " << msgSize << std::endl;
        
        if (msgSize < 0 || msgSize > 1000000) { // Sanity check on message size
            throw std::runtime_error("Invalid message size: " + std::to_string(msgSize));
        }
    }
    catch(const std::exception& e) {
        std::cerr << "Error reading message size: " << e.what() << std::endl;
        throw;
    }

    std::vector<char> buffer(msgSize);
    try {
        asio::read(socket, asio::buffer(buffer.data(), msgSize));
        std::cout << "Message data received successfully" << std::endl;
    }
    catch(const std::exception& e) {
        std::cerr << "Error reading message data: " << e.what() << std::endl;
        throw;
    }

    try {
        auto message = MockMessage::deserialize(buffer);
        std::cout << "Message deserialized successfully" << std::endl;
        return message;
    } catch (const std::exception& e) {
        std::cerr << "Error deserializing message: " << e.what() << std::endl;
        throw;
    }
}

void buildMessage(MessageBuilder<MockMessage>& messageBuilder, const std::string& messageContents, const std::string& toUsername, const int toUserID, const std::string& fromUsername, const int fromUserID, const int toChatroomID, const int fromChatroomID, const MessageType messageType) {
    messageBuilder.setMessageContents(messageContents);
    messageBuilder.setToUsername(toUsername);
    messageBuilder.setToUserID(toUserID);
    messageBuilder.setFromUsername(fromUsername);
    messageBuilder.setFromUserID(fromUserID);
    messageBuilder.setToChatroomID(toChatroomID);
    messageBuilder.setFromChatroomID(fromChatroomID);
    messageBuilder.setMessageType(messageType);
}