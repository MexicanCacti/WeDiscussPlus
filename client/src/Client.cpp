#include "Client.hpp"
#include "Message.hpp"
#include <iostream>
#include <thread>
#include <chrono>
using namespace std::chrono_literals;

Client::Client(const std::string& host, int port) 
    : _ioContext(), sendSocket(_ioContext), receiveSocket(_ioContext) {
    tcp::resolver resolver(_ioContext);
    auto endpoints = resolver.resolve(host, std::to_string(port));
    asio::connect(sendSocket, endpoints);
    asio::connect(receiveSocket, endpoints);
}

void Client::start() {
    MessageBuilder<Message> builder;
    try {
        // Send initial connection message
        builder.setMessageType(MessageType::CONNECT);
        builder.setFromUserID(1);  // Test user ID
        builder.setMessageContents("Test connection");
        Message message(&builder);
        sendMessage(message, sendSocket);

        Message receivedMessage = readMessage(sendSocket);
        std::cout << "Received message: " << receivedMessage.getMessageContents() << std::endl;
        if(receivedMessage.getMessageType() == MessageType::CONNECT) {
            std::cout << "Received CONNECT message from server:\n";
            receivedMessage.printMessage();
        } else {
            std::cerr << "Expected CONNECT message, got something else.\n";
        }

        
        builder.setMessageType(MessageType::AUTHENTICATE);
        builder.setFromUserID(1);
        builder.setMessageContents("Test authenticate");
        Message recvMessage(&builder);
        sendMessage(recvMessage, receiveSocket);

        receivedMessage = readMessage(receiveSocket);
        std::cout << "Received message: " << receivedMessage.getMessageContents() << std::endl;
        if(receivedMessage.getMessageType() == MessageType::AUTHENTICATE) { 
            std::cout << "Received AUTHENTICATE message from server:\n";
            receivedMessage.printMessage();
        } else {
            std::cerr << "Expected AUTHENTICATE message, got something else.\n";
        }


        // Start receiving messages in a separate thread
        std::thread receiveThread([this]() {
            while (true) {
                try {
                    Message receivedMessage = readMessage(receiveSocket);
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

            MessageBuilder<Message> msgBuilder;
            msgBuilder.setMessageType(MessageType::UNDEFINED);
            msgBuilder.setFromUserID(1);
            msgBuilder.setMessageContents(input);
            Message msg(&msgBuilder);
            sendMessage(msg, sendSocket);
        }

        receiveThread.join();
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void Client::sendMessage(const Message& message, tcp::socket& socket) {
    std::cout << "Sending message: " << message.getMessageContents() << std::endl;
    std::vector<char> serializedMessage = message.serialize();
    asio::write(socket, asio::buffer(serializedMessage));
}

Message Client::readMessage(tcp::socket& socket) {
    int msgSize = 0;
    asio::read(socket, asio::buffer(&msgSize, sizeof(int)));
    
    if (msgSize <= 0) {
        throw std::runtime_error("Invalid message size received: " + std::to_string(msgSize));
    }
    
    std::vector<char> buffer(msgSize);
    asio::read(socket, asio::buffer(buffer.data(), msgSize));
    return Message::deserialize(buffer);
}
