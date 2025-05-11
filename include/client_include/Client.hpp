#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <asio.hpp>
#include <string>
#include <vector>
#include "shared_include/Message.hpp"

using asio::ip::tcp;

class Client {
    private:
        asio::io_context _ioContext;
        tcp::socket _socket;

    public:
        Client(const std::string& host, int port);
        void start();
        void sendMessage(const Message& message);
        Message readMessage();
};

#endif