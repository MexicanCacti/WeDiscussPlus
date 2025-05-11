#ifndef SERVER_HPP
#define SERVER_HPP

#include <atomic>
#include <thread>
#include <iostream>
#include <asio.hpp>
#include <memory>
#include <mutex>
#include <unordered_map>

#include "Message.hpp"
#include "LoadBalancer.hpp"
#include "ChatroomManagerBalancer.hpp"
#include "UserManagerBalancer.hpp"
#include "LogManagerBalancer.hpp"

using asio::ip::tcp;

template<typename WorkType>
class Server {
    private:
        struct ClientSockets {
            std::shared_ptr<tcp::socket> fromSocket;
            std::shared_ptr<tcp::socket> toSocket;
        };

        int _port;
        bool _isRunning = false;
        asio::io_context _ioContext;
        tcp::acceptor _connectionAcceptor;
        std::mutex _clientSocketMapMutex;
        std::unordered_map<int, ClientSockets> _clientSocketMap;

        std::unique_ptr<ChatroomManagerBalancer<WorkType>> _chatroomManagerBalancer;
        std::unique_ptr<UserManagerBalancer<WorkType>> _userManagerBalancer;
        std::unique_ptr<LogManagerBalancer<WorkType>> _logManagerBalancer;

        int _MAX_CHATROOM_MANAGERS = 2;
        int _MAX_USER_MANAGERS = 2;
        int _MAX_LOG_MANAGERS = 2;

        void listenForConnections();
        void registerClient(std::shared_ptr<tcp::socket> clientSocket);
        void registerToClientSocket(int clientID, std::shared_ptr<tcp::socket> clientSocket);
        void handleClient(int clientID);
        WorkType readMessageFromSocket(std::shared_ptr<tcp::socket> socket);
        void sendMessageToSocket(std::shared_ptr<tcp::socket> socket, WorkType& message);
        void shutdown();

    public:
        Server(const std::string& ip, int port);
        ~Server() {shutdown();}
        inline void setMaxChatroomManagers(int maxChatroomManagers) {_MAX_CHATROOM_MANAGERS = maxChatroomManagers;}
        inline void setMaxUserManagers(int maxUserManagers) {_MAX_USER_MANAGERS = maxUserManagers;}
        inline void setMaxLogManagers(int maxLogManagers) {_MAX_LOG_MANAGERS = maxLogManagers;}
        void startServer();
        void stopServer();
        void addMessageToLogBalancer(WorkType& message);
        inline bool isRunning() const {return _isRunning;}
        void addFromClientSocket(int clientID, std::shared_ptr<tcp::socket> fromSocket);
        void addToClientSocket(int clientID, std::shared_ptr<tcp::socket> toSocket);
        ClientSockets getClientSocket(int clientID);
};

#endif