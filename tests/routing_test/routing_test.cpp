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

bool successBit = true;
std::string messageContents = "Test Contents";
std::string toUsername = "Test ToUsername";
int toUserID = 99; 
std::string fromUsername = "Test FromUsername";
int fromUserID = 100;
int toChatroomID = 0;
int fromChatroomID = 0;

void buildMessage(MessageBuilder<MockMessage>&, const bool, const std::string&, const std::string&, const int, const std::string&, const int, const int, const int, const MessageType);
MockMessage readMessageFromSocket(tcp::socket& socket);

class RoutingTest : public ::testing::Test {
protected:
    static std::unique_ptr<Server<MockMessage>> _server;
    static std::thread _serverThread;

    static void SetUpTestSuite() {
        try {
            _server = std::make_unique<Server<MockMessage>>("127.0.0.1", 3333);
            
            std::promise<void> serverStarted;
            std::future<void> serverStartedFuture = serverStarted.get_future();
            
            std::cout << "Creating server thread..." << std::endl;
            _serverThread = std::thread([&serverStarted](){
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
                _serverThread.detach();  // Detach the thread so it can run independently
            } catch (const std::exception& e) {
                std::cerr << "Failed to start server: " << e.what() << std::endl;
                throw;
            }
        } catch (const std::exception& e) {
            std::cerr << "Error in SetUpTestSuite: " << e.what() << std::endl;
            throw;
        }
    }

    static void TearDownTestSuite() {
        std::cout << "Stopping Server..." << std::endl;
        _server->stopServer();
        std::cout << "Server Stopped" << std::endl;
    }

    // sendSocket -> Socket that sends messages to server, recvSocket -> Socket that receives messages from server
    void establishConnection(tcp::socket& sendSocket, tcp::socket& recvSocket) {
        std::cout << "Starting connection establishment..." << std::endl;
        try {
            sendSocket.connect(tcp::endpoint(asio::ip::address::from_string("127.0.0.1"), 3333));
            std::cout << "Initial socket created successfully" << std::endl;

            MessageBuilder<MockMessage> sendBuilder;
            buildMessage(sendBuilder, successBit, "CONNECT", toUsername, toUserID, fromUsername, fromUserID, toChatroomID, fromChatroomID, MessageType::CONNECT);
            MockMessage sendMessage(&sendBuilder);
            std::cout << "SEND MESSAGE BUILT: MESSAGE TYPE: " << MockMessage::messageTypeToString(sendMessage.getMessageType()) << std::endl;
            std::vector<char> sendData = sendMessage.serialize();
            asio::write(sendSocket, asio::buffer(sendData));
            std::cout << "CONNECT message sent successfully" << std::endl;

            MockMessage connectResponse = readMessageFromSocket(sendSocket);
            EXPECT_EQ(connectResponse.getMessageType(), MessageType::CONNECT);
            EXPECT_EQ(connectResponse.getMessageContents(), "server");
            std::cout << "Connect response verified successfully" << std::endl;
            fromUserID = connectResponse.getFromUserID();

            recvSocket.connect(tcp::endpoint(asio::ip::address::from_string("127.0.0.1"), 3333));
            std::cout << "Receive socket created successfully" << std::endl;

            MessageBuilder<MockMessage> recvBuilder;
            buildMessage(recvBuilder, successBit, "AUTHENTICATE", toUsername, toUserID, fromUsername, fromUserID, toChatroomID, fromChatroomID, MessageType::AUTHENTICATE);
            MockMessage recvMessage(&recvBuilder);
            std::vector<char> recvData = recvMessage.serialize();
            asio::write(recvSocket, asio::buffer(recvData));
            std::cout << "AUTHENTICATE message sent successfully" << std::endl;

            MockMessage authenticateResponse = readMessageFromSocket(recvSocket);
            EXPECT_EQ(authenticateResponse.getMessageType(), MessageType::AUTHENTICATE);
            EXPECT_EQ(authenticateResponse.getMessageContents(), "authUser");
            std::cout << "AUTHENTICATE response verified successfully" << std::endl;

            std::cout << "Connection establishment completed" << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Error in establishConnection: " << e.what() << std::endl;
            throw;
        }
    }

private:
};

// Static member definitions for RoutingTest
std::unique_ptr<Server<MockMessage>> RoutingTest::_server;
std::thread RoutingTest::_serverThread;

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
            {MessageType::AUTHENTICATE, "authUser"},
            {MessageType::CONNECT, "server"}
        };

        for(const auto& [messageType, expectedFunction] : userManagerTypes) {
            std::cout << "\nTesting message type: " << MockMessage::messageTypeToString(messageType) << std::endl;
            
            MessageBuilder<MockMessage> builder;
            buildMessage(builder, successBit, messageContents, toUsername, toUserID, fromUsername, fromUserID, toChatroomID, fromChatroomID, messageType);
            MockMessage message(&builder);
            std::vector<char> data = message.serialize();
            
            try {
                asio::write(sendSocket, asio::buffer(data));
                std::cout << "Message sent successfully" << std::endl;
            } catch (const std::exception& e) {
                std::cerr << "FAILED to send message: " << e.what() << std::endl;
                throw;
            }

            try {
                MockMessage response = readMessageFromSocket(recvSocket);
                EXPECT_EQ(response.getMessageContents(), expectedFunction);
                std::cout << "SUCCESS: " << response.getMessageContents() << " | " << expectedFunction << std::endl;
            } catch (const std::exception& e) {
                std::cerr << "FAILED to read response: " << e.what() << std::endl;
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
    std::cout << "Starting ChatroomManager routing test" << std::endl;
    try{
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
            buildMessage(builder, successBit, messageContents, toUsername, toUserID, fromUsername, fromUserID, toChatroomID, fromChatroomID, messageType);
            MockMessage message(&builder);
            std::vector<char> data = message.serialize();
            try {
                asio::write(sendSocket, asio::buffer(data));
                std::cout << "Message sent successfully" << std::endl;
            } catch (const std::exception& e) {
                std::cerr << "FAILED to send message: " << e.what() << std::endl;
                throw;
            }

            try {
                MockMessage response = readMessageFromSocket(recvSocket);
                EXPECT_EQ(response.getMessageContents(), expectedFunction);
                std::cout << "SUCCESS: " << response.getMessageContents() << " | " << expectedFunction << std::endl;
            } catch (const std::exception& e) {
                std::cerr << "FAILED to read response: " << e.what() << std::endl;
                throw;
            }
        }
    } catch(const std::exception& e){
        std::cerr << "Error in ChatroomManager test: " << e.what() << std::endl;
        throw;
    }

}

TEST_F(RoutingTest, LogManagerRouting) {
    std::cout << "Starting LogManager routing test" << std::endl;
    try{
        tcp::socket sendSocket(context);
        tcp::socket recvSocket(context);
        establishConnection(sendSocket, recvSocket);

        std::vector<std::pair<MessageType, std::string>> logManagerTypes = {
            {MessageType::GET_USER_MESSAGES, "getUserMessages"},
            {MessageType::GET_CHATROOM_MESSAGES, "getChatroomMessages"}
        };

        for(const auto& [messageType, expectedFunction] : logManagerTypes) {
            MessageBuilder<MockMessage> builder;
            buildMessage(builder, successBit, messageContents, toUsername, toUserID, fromUsername, fromUserID, toChatroomID, fromChatroomID, messageType);
            MockMessage message(&builder);
            std::vector<char> data = message.serialize();
            try {
                asio::write(sendSocket, asio::buffer(data));
                std::cout << "Message sent successfully" << std::endl;
            } catch (const std::exception& e) {
                std::cerr << "FAILED to send message: " << e.what() << std::endl;
                throw;
            }

            try {
                MockMessage response = readMessageFromSocket(recvSocket);
                EXPECT_EQ(response.getMessageContents(), expectedFunction);
                std::cout << "SUCCESS: " << response.getMessageContents() << " | " << expectedFunction << std::endl;
            } catch (const std::exception& e) {
                std::cerr << "FAILED to read response: " << e.what() << std::endl;
                throw;
            }
        }
    }
    catch(const std::exception& e){
        std::cerr << "Error in ChatroomManager test: " << e.what() << std::endl;
        throw;
    }    
}

MockMessage readMessageFromSocket(tcp::socket& socket) {
    int msgSize = 0;
    try {
        asio::read(socket, asio::buffer(&msgSize, sizeof(int)));
        
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

void buildMessage(MessageBuilder<MockMessage>& messageBuilder, const bool successBit, const std::string& messageContents, const std::string& toUsername, const int toUserID, const std::string& fromUsername, const int fromUserID, const int toChatroomID, const int fromChatroomID, const MessageType messageType) {
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