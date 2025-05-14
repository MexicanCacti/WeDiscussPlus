#ifndef SERVER_HPP
#define SERVER_HPP

#include <atomic>
#include <thread>
#include <iostream>
#include <asio.hpp>
#include <memory>
#include <mutex>
#include <optional>
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
        // Core server state
        int _port;
        bool _isRunning = false;
        asio::io_context _ioContext;
        tcp::acceptor _connectionAcceptor;
        
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
        std::unique_ptr<ChatroomManagerBalancer<WorkType>> _chatroomManagerBalancer;
        std::unique_ptr<UserManagerBalancer<WorkType>> _userManagerBalancer;
        std::unique_ptr<LogManagerBalancer<WorkType>> _logManagerBalancer;

        // Configuration
        int _MAX_CHATROOM_MANAGERS = 2;
        int _MAX_USER_MANAGERS = 2;
        int _MAX_LOG_MANAGERS = 2;

        // Core server operations
        void listenForConnections();
        void shutdown();
        
        // Client registration
        void registerClient(std::shared_ptr<tcp::socket> clientSocket);
        void registerFromClientSocket(std::shared_ptr<tcp::socket> clientSocket, WorkType& message);
        
        // Socket operations
        std::optional<WorkType> readMessageFromSocket(tcp::socket& socket);
        bool sendMessageToSocket(tcp::socket& socket, WorkType& message);
        

    public:
        Server(const std::string& ip, int port);
        ~Server() {shutdown();}

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
        bool sendMessageToClient(int clientID, WorkType& message);
        std::optional<WorkType> readMessageFromClient(int clientID);
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
        void addMessageToLogBalancer(WorkType& message);
};

#endif