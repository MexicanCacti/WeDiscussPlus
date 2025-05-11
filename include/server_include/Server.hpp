#ifndef SERVER_HPP
#define SERVER_HPP

#include <atomic>
#include <thread>
#include <iostream>
#include <asio.hpp>

#include "Message.hpp"
#include "LoadBalancer.hpp"
#include "ChatroomManagerBalancer.hpp"
#include "UserManagerBalancer.hpp"
#include "LogManagerBalancer.hpp"

using asio::ip::tcp;

template<typename WorkType>
class Server {
    int _MAX_LOG_MANAGERS = 2;
    int _MAX_USER_MANAGERS = 2;
    int _MAX_CHATROOM_MANAGERS = 2;

    protected:
        std::unique_ptr<ChatroomManagerBalancer<WorkType>> _chatroomManagerBalancer = nullptr;
        std::unique_ptr<UserManagerBalancer<WorkType>> _userManagerBalancer = nullptr;
        std::unique_ptr<LogManagerBalancer<WorkType>> _logManagerBalancer = nullptr;
        
        struct ClientSockets{
            std::shared_ptr<tcp::socket> fromSocket;
            std::shared_ptr<tcp::socket> toSocket;
        };
        // Map | ClientID : To&From Sockets
        std::unordered_map<int, ClientSockets> _clientSocketMap;
        std::mutex _clientSocketMapMutex;
        asio::io_context _ioContext;

    private:
        int _port;
        std::string _serverIP = "";
        tcp::acceptor _connectionAcceptor;
        bool _isRunning = true;

        void listenForConnections();
        void handleClient(int clientID);
        void registerClient(std::shared_ptr<tcp::socket> clientSocket);
        void registerToClientSocket(int clientID,std::shared_ptr<tcp::socket> clientSocket);
        WorkType readMessageFromSocket(std::shared_ptr<tcp::socket> socket);
        void sendMessageToSocket(std::shared_ptr<tcp::socket> socket, WorkType& message);
        void shutdown();
    public:
        Server(int port);
        ~Server() {shutdown();}
        inline void setMaxChatroomManagers(int maxChatroomManagers) {_MAX_CHATROOM_MANAGERS = maxChatroomManagers;}
        inline void setMaxUserManagers(int maxUserManagers) {_MAX_USER_MANAGERS = maxUserManagers;}
        inline void setMaxLogManagers(int maxLogManagers) {_MAX_LOG_MANAGERS = maxLogManagers;}
        void startServer();
        void stopServer();
        void addMessageToLogBalancer(WorkType& message);
        inline bool isRunning() const {return _isRunning;}
};

#endif