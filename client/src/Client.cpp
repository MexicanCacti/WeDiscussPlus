#include "client_include/Client.hpp"
#include "shared_include/Message.hpp"
#include <iostream>
#include <thread>
#include <chrono>
using namespace std::chrono_literals;

Client::Client(const std::string& host, int port) 
    : _ioContext(), _socket(_ioContext) {
    tcp::resolver resolver(_ioContext);
    auto endpoints = resolver.resolve(host, std::to_string(port));
    asio::connect(_socket, endpoints);
}

void Client::start() {
    try {
        // Send initial connection message
        MessageBuilder builder;
        builder.setMessageType(MessageType::SEND);
        builder.setFromUserID(1);  // Test user ID
        builder.setMessageContents("Test connection");
        Message message(&builder);
        sendMessage(message);

        // Start receiving messages in a separate thread
        std::thread receiveThread([this]() {
            while (true) {
                try {
                    Message receivedMessage = readMessage();
                    std::cout << "Received message: " << receivedMessage.getMessageContents() << std::endl;
                }
                catch (const std::exception& e) {
                    std::cerr << "Error receiving message: " << e.what() << std::endl;
                    break;
                }
            }
        });

        // Main thread sends messages
        while (true) {
            std::string input;
            std::cout << "Enter message (or 'quit' to exit): ";
            std::getline(std::cin, input);
            
            if (input == "quit") break;

            MessageBuilder msgBuilder;
            msgBuilder.setMessageType(MessageType::SEND);
            msgBuilder.setFromUserID(1);
            msgBuilder.setMessageContents(input);
            Message msg(&msgBuilder);
            sendMessage(msg);
        }

        receiveThread.join();
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void Client::sendMessage(const Message& message) {
    std::cout << "Sending message: " << message.getMessageContents() << std::endl;
    std::vector<char> serializedMessage = message.serialize();
    asio::write(_socket, asio::buffer(serializedMessage));
}

Message Client::readMessage() {
    int msgSize = 0;
    asio::read(_socket, asio::buffer(&msgSize, sizeof(int)));
    
    if (msgSize <= 0) {
        throw std::runtime_error("Invalid message size received: " + std::to_string(msgSize));
    }
    
    std::vector<char> buffer(msgSize);
    asio::read(_socket, asio::buffer(buffer.data(), msgSize));
    return Message::deserialize(buffer);
}
