#ifndef SERVER_HPP
#define SERVER_HPP

#include <atomic>
#include <thread>
#include <iostream>
#include <asio.hpp>

#include "shared_include/Message.hpp"
#include "server_include/LoadBalancer.hpp"
#include "server_include/ChatroomManagerBalancer.hpp"
#include "server_include/UserManagerBalancer.hpp"
#include "server_include/LogManagerBalancer.hpp"

using asio::ip::tcp;

class Server {
    const int _MAX_LOG_MANAGERS = 2;
    const int _MAX_USER_MANAGERS = 2;
    const int _MAX_CHATROOM_MANAGERS = 2;

    std::unique_ptr<ChatroomManagerBalancer<Message>> _chatroomManagerBalancer = nullptr;
    std::unique_ptr<UserManagerBalancer<Message>> _userManagerBalancer = nullptr;
    std::unique_ptr<LogManagerBalancer<Message>> _logManagerBalancer = nullptr;

    private:
        struct ClientSockets{
            std::shared_ptr<tcp::socket> fromSocket;
            std::shared_ptr<tcp::socket> toSocket;
        };

        int _port;
        std::string _serverIP = "";
        bool _isRunning = true;
        asio::io_context _ioContext;
        tcp::acceptor _connectionAcceptor;

        // Map | ClientID : To&From Sockets
        std::unordered_map<int, ClientSockets> _clientSocketMap;
        std::mutex _clientSocketMapMutex;

        void listenForConnections();
        void handleClient(int clientID);
        void registerClient(std::shared_ptr<tcp::socket> clientSocket);
        void registerToClientSocket(int clientID,std::shared_ptr<tcp::socket> clientSocket);
        void registerFromClientSocket(std::shared_ptr<tcp::socket> clientSocket);
        Message readMessageFromSocket(std::shared_ptr<tcp::socket> socket);
        void sendMessageToSocket(std::shared_ptr<tcp::socket> socket, Message& message);
        void shutdown();
    public:
        Server(int port);
        ~Server() {shutdown();}
        void startServer();
        void stopServer();
        void addMessageToLogBalancer(Message& message);
};

#endif