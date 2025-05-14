#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <asio.hpp>
#include <string>
#include <vector>
#include "Message.hpp"

using asio::ip::tcp;

class Client {
    private:
        asio::io_context _ioContext;
        tcp::socket sendSocket;
        tcp::socket receiveSocket;

    public:
        Client(const std::string& host, int port);
        void start();
        void sendMessage(const Message& message, tcp::socket& socket);
        Message readMessage(tcp::socket& socket);
};

#endif