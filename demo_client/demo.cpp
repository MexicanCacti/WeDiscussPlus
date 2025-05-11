#include <iostream>
#include <asio.hpp>
#include <thread>
#include "shared_include/Message.hpp"
#include "shared_include/MessageBuilder.hpp"

using asio::ip::tcp;

void sendMessage(tcp::socket& socket, Message& message) {
    std::vector<char> data = message.serialize();
    asio::write(socket, asio::buffer(data));
}

Message receiveMessage(tcp::socket& socket) {
    std::cout << "Waiting to receive echoed message...\n";
    int size;
    asio::read(socket, asio::buffer(&size, sizeof(int)));
    std::vector<char> buffer(size);
    asio::read(socket, asio::buffer(buffer.data(), size));
    return Message::deserialize(buffer);
}

void listenForMessages(tcp::socket& recvSocket) {
    while (true) {
        // Receive message from the server
        Message echoed = receiveMessage(recvSocket);
        std::cout << "Received response from server:\n";
        echoed.printMessage();
    }
}

void sendMessages(tcp::socket& sendSocket, int clientID) {
    // Send a test message
    int messageNumber = 0;
    MessageBuilder<Message> testBuilder;
    testBuilder.setMessageType(MessageType::UNDEFINED);
    testBuilder.setFromUserID(clientID);
    testBuilder.setToUserID(clientID);
    testBuilder.setMessageContents("Hello Server!");
    Message testMessage(&testBuilder);

    while(true){
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        sendMessage(sendSocket, testMessage);
        std::cout << "Sent Message: ";
        testMessage.printMessage();
    }
}

int main() {
    asio::io_context context;
    tcp::resolver resolver(context);
    auto endpoints = resolver.resolve("127.0.0.1", "333");

    int clientID = 42;  // Use any unique ID

    // Connect SEND socket
    tcp::socket sendSocket(context); // client sends messages to the server with this socket TO SERVER
    asio::connect(sendSocket, endpoints);

    MessageBuilder<Message> sendBuilder;
    sendBuilder.setMessageType(MessageType::SEND);
    sendBuilder.setMessageContents("I sent a SEND message!");
    sendBuilder.setFromUserID(clientID);
    Message sendRegMsg(&sendBuilder);
    sendMessage(sendSocket, sendRegMsg);
    std::cout << "Sent Message: ";
    sendRegMsg.printMessage();

    // Connect RECV socket
    tcp::socket recvSocket(context);
    asio::connect(recvSocket, endpoints);

    MessageBuilder<Message> recvBuilder; // server sends messages to this socket TO CLIENT
    recvBuilder.setMessageType(MessageType::RECV);
    recvBuilder.setMessageContents("I sent a RECV message!");
    recvBuilder.setFromUserID(clientID);
    Message recvRegMsg(&recvBuilder);
    sendMessage(recvSocket, recvRegMsg);

    std::cout << "Sent Message: "; 
    recvRegMsg.printMessage();

    // Wait for SYSTEM ACK from server
    Message ack = receiveMessage(recvSocket);
    if (ack.getMessageType() == MessageType::ACK) {
        std::cout << "Received ACK from server:\n";
        ack.printMessage();
    } else {
        std::cerr << "Expected ACK, got something else.\n";
        return 1;
    }
    // Start listening for incoming messages in a separate thread
    std::thread listenThread(listenForMessages, std::ref(recvSocket));

    // Send messages in the main thread
    sendMessages(sendSocket, clientID);

    // Wait for the listening thread to finish
    listenThread.join();

    return 0;
}