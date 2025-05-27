#include <iostream>
#include <asio.hpp>
#include <thread>
#include <optional>
#include "MessageInterface.hpp"
#include "MessageBuilder.hpp"
#include "MessageFactory.hpp"

using asio::ip::tcp;

bool success = false;

bool sendMessageToSocket(tcp::socket& socket, std::shared_ptr<MessageInterface> message) {
    try {
        std::vector<char> data = message->serialize();
        int msgSize = static_cast<int>(data.size());
        asio::write(socket, asio::buffer(&msgSize, sizeof(int)));
        asio::write(socket, asio::buffer(data));
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error sending message to socket: " << e.what() << std::endl;
        return false;
    }
}

std::optional<std::shared_ptr<MessageInterface>> readMessageFromSocket(tcp::socket& socket) {
    try {
        int msgSize = 0;
        asio::read(socket, asio::buffer(&msgSize, sizeof(int)));
        
        if (msgSize < 0 || msgSize > 1000000) { // Sanity check on message size
            throw std::runtime_error("Invalid message size: " + std::to_string(msgSize));
        }

        std::vector<char> buffer(msgSize);
        asio::read(socket, asio::buffer(buffer.data(), msgSize));
        
        #ifdef _DEBUG
        std::cout << "Message data received successfully, size: " << msgSize << std::endl;
        #endif

        size_t offset = 0;
        auto message = MessageFactory::deserialize(buffer, offset);
        
        #ifdef _DEBUG
        std::cout << "Message deserialized successfully, type: " << static_cast<int>(message->getMessageType()) << std::endl;
        #endif

        return message;
    } catch (const std::exception& e) {
        std::cerr << "Error reading message from socket: " << e.what() << std::endl;
        return std::nullopt;
    }
}

void listenForMessages(tcp::socket& recvSocket) {
    while (true) {
        // Receive message from the server
        std::optional<std::shared_ptr<MessageInterface>> echoed = readMessageFromSocket(recvSocket);
        if (echoed) {
            std::cout << "Received response from server:\n";
            echoed.value()->printMessage();
        }
        else throw std::runtime_error("Failed to receive message from server");
    }
}

void sendMessages(tcp::socket& sendSocket, int clientID) {
    // Send a test message
    int messageNumber = 0;
    MessageBuilder testBuilder;
    testBuilder.setMessageType(MessageType::SEND_MESSAGE_TO_USER);
    testBuilder.setFromUser("Client", clientID);
    testBuilder.setToUser("Server", 0);
    testBuilder.setMessageContents("Hello Server!");
    std::shared_ptr<MessageInterface> testMessage = testBuilder.build();

    while(true){
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        success = sendMessageToSocket(sendSocket, testMessage);
        if (!success) {
            std::cerr << "Failed to send message" << std::endl;
            break;
        }
        std::cout << "Sent Message: ";
        testMessage->printMessage();
    }
}

int main() {
    asio::io_context context;
    tcp::resolver resolver(context);
    auto endpoints = resolver.resolve("127.0.0.1", "3333");

    int clientID = 42;  // Use any unique ID

    // Connect SEND socket
    tcp::socket sendSocket(context); // client sends messages to the server with this socket TO SERVER
    asio::connect(sendSocket, endpoints);

    MessageBuilder sendBuilder;
    sendBuilder.setMessageType(MessageType::CONNECT);
    sendBuilder.setSuccessBit(true);
    sendBuilder.setMessageContents("I sent a CONNECT message!");
    sendBuilder.setFromUser("Client", clientID);
    sendBuilder.setToUser("Server", clientID);
    std::shared_ptr<MessageInterface> sendRegMsg = sendBuilder.build();
    success = sendMessageToSocket(sendSocket, sendRegMsg);
    if (!success) {
        std::cerr << "Failed to send message" << std::endl;
        return 1;
    }
    std::cout << "Sent Message: ";
    sendRegMsg->printMessage();

    // Wait for CONNECT response
    std::optional<std::shared_ptr<MessageInterface>> connectMessage = readMessageFromSocket(sendSocket);
    if (!connectMessage) {
        std::cerr << "Failed to receive message" << std::endl;
        return 1;
    }
    if (connectMessage.value()->getMessageType() == MessageType::CONNECT) {
        std::cout << "Received CONNECT message from server:\n";
        connectMessage.value()->printMessage();
        clientID = connectMessage.value()->getFromUserID();
    } else {
        std::cerr << "Expected CONNECT message, got something else.\n";
        return 1;
    }

    // Connect RECV socket
    tcp::socket recvSocket(context);
    asio::connect(recvSocket, endpoints);

    // Now send AUTHENTICATE
    MessageBuilder recvBuilder;
    recvBuilder.setMessageType(MessageType::AUTHENTICATE);
    recvBuilder.setMessageContents("I sent a AUTHENTICATE message!");
    recvBuilder.setFromUser("Server", clientID);
    recvBuilder.setToUser("Client", clientID);
    std::shared_ptr<MessageInterface> recvRegMsg = recvBuilder.build();
    success = sendMessageToSocket(recvSocket, recvRegMsg);
    if (!success) {
        std::cerr << "Failed to send message" << std::endl;
        return 1;
    }
    std::cout << "Sent Message: "; 
    recvRegMsg->printMessage();

    // Wait for AUTHENTICATE response
    std::optional<std::shared_ptr<MessageInterface>> ack = readMessageFromSocket(recvSocket);
    if (!ack) {
        std::cerr << "Failed to receive message" << std::endl;
        return 1;
    }
    if (ack.value()->getMessageType() == MessageType::AUTH_RESPONSE) {
        std::cout << "Received AUTH_RESPONSE from server:\n";
        clientID = ack.value()->getToUserID();
        ack.value()->printMessage();
    } else {
        std::cerr << "Expected AUTH_RESPONSE, got something else.\n";
        return 1;
    }

    // Give server time to start client handler thread
    std::this_thread::sleep_for(std::chrono::milliseconds(2500));
    // Start listening for incoming messages in a separate thread
    std::thread listenThread(listenForMessages, std::ref(recvSocket));

    // Send messages in the main thread
    sendMessages(sendSocket, clientID);

    // Wait for the listening thread to finish
    listenThread.join();

    return 0;
}