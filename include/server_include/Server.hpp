#pragma once

#include <atomic>
#include <thread>
#include <iostream>
#include <asio.hpp>
#include <memory>
#include <mutex>
#include <optional>
#include <unordered_map>

#include "MessageInterface.hpp"
#include "MessageBuilder.hpp"
#include "MessageFactory.hpp"
#include "LoadBalancer.hpp"
#include "ChatroomManagerBalancer.hpp"
#include "UserManagerBalancer.hpp"
#include "LogManagerBalancer.hpp"
#include "Database.hpp"

using asio::ip::tcp;

class Server {
    private:
        // Core server state
        int _port;
        bool _isRunning = false;
        asio::io_context _ioContext;
        tcp::acceptor _connectionAcceptor;
        std::unique_ptr<Database> _db; // TODO: Init Database in constructor

        // Socket management
        static inline int _tempClientID = 0; // Temp Ids decrement, wrap back when at limit. When Assigning: --_tempClientID
        struct ClientSockets {
            std::shared_ptr<tcp::socket> fromSocket;
            std::shared_ptr<tcp::socket> toSocket;
        };
        std::unordered_map<int, ClientSockets> _clientSocketMap;
        std::unordered_map<int, std::shared_ptr<std::mutex>> _clientSocketMutexMap; // Whenever a FROM socket added, a mutex is added to the map.
        std::mutex _clientSocketMutexMapMutex; // Mutex for protecting _clientSocketMutexMap access

        // Manager balancers
        std::unique_ptr<ChatroomManagerBalancer> _chatroomManagerBalancer;
        std::unique_ptr<UserManagerBalancer> _userManagerBalancer;
        std::unique_ptr<LogManagerBalancer> _logManagerBalancer;

        // Configuration
        int _MAX_CHATROOM_MANAGERS = 2;
        int _MAX_USER_MANAGERS = 2;
        int _MAX_LOG_MANAGERS = 2;

        // Core server operations
        void listenForConnections();
        
        // Client registration
        void registerClient(std::shared_ptr<tcp::socket> clientSocket);
        void registerFromClientSocket(std::shared_ptr<tcp::socket> clientSocket, std::shared_ptr<MessageInterface> message);
        
        // Socket operations
        std::optional<std::shared_ptr<MessageInterface>> readMessageFromSocket(tcp::socket& socket);
        bool sendMessageToSocket(tcp::socket& socket, std::shared_ptr<MessageInterface> message);
        bool sendConnectMessage(int clientID, std::shared_ptr<MessageInterface> message);
        

    public:
        Server(const std::string& ip, int port, const std::string& dbPath, const std::string& schemaPath);
        ~Server() {stopServer();}
        Database* getDatabase() {return _db.get();}

        // Core server operations
        void startServer();
        void stopServer();
        inline bool isRunning() const {return _isRunning;}
        void handleClient(int clientID);

        // Configuration
        inline void setMaxChatroomManagers(int maxChatroomManagers) {_MAX_CHATROOM_MANAGERS = maxChatroomManagers;}
        inline void setMaxUserManagers(int maxUserManagers) {_MAX_USER_MANAGERS = maxUserManagers;}
        inline void setMaxLogManagers(int maxLogManagers) {_MAX_LOG_MANAGERS = maxLogManagers;}

        // Socket management
        bool addFromClientSocket(int clientID, std::shared_ptr<tcp::socket> fromSocket);
        bool addToClientSocket(int clientID, std::shared_ptr<tcp::socket> toSocket);
        bool sendMessageToClient(int clientID, std::shared_ptr<MessageInterface> message);
        std::optional<std::shared_ptr<MessageInterface>> readMessageFromClient(int clientID);
        bool removeClientSocket(int clientID);
        
        // Socket status checks
        bool isFromClientSocketRegistered(int clientID);
        bool isToClientSocketRegistered(int clientID);
        std::string getClientSocketIP(int clientID); 

        // Mutex management
        std::optional<std::shared_ptr<std::mutex>> getClientSocketMutex(int clientID);
        bool addClientSocketMutex(int clientID, std::shared_ptr<std::mutex> socketMutex);
        inline bool hasClientSocketMutex(int clientID) {return _clientSocketMutexMap.find(clientID) != _clientSocketMutexMap.end();}
        bool removeClientSocketMutex(int clientID); // Only called when remove Client Socket called or when client auth called

        // Message handling
        void addMessageToLogBalancer(std::shared_ptr<MessageInterface> message);
};