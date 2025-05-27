#include "Server.hpp"

// Core Server Operations

Server::Server(const std::string& ip, int port) 
    : _port(port), 
      _ioContext(),
      _connectionAcceptor(_ioContext, tcp::endpoint(asio::ip::make_address(ip), port)) {}

void Server::startServer(){
    try {
        #ifdef _DEBUG
        std::cout << "Server: Starting initialization..." << std::endl;
        #endif
        
        _chatroomManagerBalancer = std::make_unique<ChatroomManagerBalancer>();
        _userManagerBalancer = std::make_unique<UserManagerBalancer>();
        _logManagerBalancer = std::make_unique<LogManagerBalancer>();
        #ifdef _DEBUG
        std::cout << "Server: Manager balancers created successfully" << std::endl;
        #endif
        
        _chatroomManagerBalancer->initManagers(_MAX_CHATROOM_MANAGERS, *this);
        #ifdef _DEBUG
        std::cout << "Server: Chatroom managers initialized" << std::endl;
        #endif
        
        _userManagerBalancer->initManagers(_MAX_USER_MANAGERS, *this);
        #ifdef _DEBUG
        std::cout << "Server: User managers initialized" << std::endl;
        #endif
        
        _logManagerBalancer->initManagers(_MAX_LOG_MANAGERS, *this);
        #ifdef _DEBUG
        std::cout << "Server: Log managers initialized" << std::endl;
        #endif

        std::thread([this]() {
            try {
                _chatroomManagerBalancer->waitForWork();
            } catch (const std::exception& e) {
                std::cerr << "ChatroomManagerBalancer thread exception: " << e.what() << std::endl;
            }
        }).detach();

        std::thread([this]() {
            try {
                _userManagerBalancer->waitForWork();
            } catch (const std::exception& e) {
                std::cerr << "UserManagerBalancer thread exception: " << e.what() << std::endl;
            }
        }).detach();

        std::thread([this]() {
            try {
                _logManagerBalancer->waitForWork();
            } catch (const std::exception& e) {
                std::cerr << "LogManagerBalancer thread exception: " << e.what() << std::endl;
            }
        }).detach();
        
        listenForConnections();

    } catch (const std::exception& e) {
        std::cerr << "Error starting server: " << e.what() << std::endl;
        throw;
    }
}

void Server::stopServer(){
    _isRunning = false;
    
    if (_chatroomManagerBalancer) _chatroomManagerBalancer->stopAllThreads();
    if (_userManagerBalancer) _userManagerBalancer->stopAllThreads();
    if (_logManagerBalancer) _logManagerBalancer->stopAllThreads();
    
    if (_connectionAcceptor.is_open()) {
        _connectionAcceptor.close();
    }
    
    _ioContext.stop();
}

void Server::listenForConnections(){
    try {
        tcp::endpoint localEndpoint = _connectionAcceptor.local_endpoint();
        #ifdef _DEBUG
        std::cout << "Server: Listening on IP: " << localEndpoint.address().to_string() 
                  << " (" << (localEndpoint.address().is_v4() ? "IPv4" : "IPv6") << ")"
                  << " Port: " << localEndpoint.port() << std::endl;
        #endif
        _isRunning = true;

        std::thread([this]() {
            while(_isRunning){
                try {
                    auto clientSocket = std::make_shared<tcp::socket>(_ioContext);
                    _connectionAcceptor.accept(*clientSocket);
                    #ifdef _DEBUG
                    std::cout << "Server: New connection accepted" << std::endl;
                    #endif
                    
                    std::thread([this, socket = std::move(clientSocket)]() mutable {
                        this->registerClient(std::move(socket));
                    }).detach();
                } catch (const std::exception& e) {
                    if (_isRunning) {
                        std::cerr << "Error accepting connection: " << e.what() << std::endl;
                    }
                }
            }
        }).detach();
        
    } catch (const std::exception& e) {
        std::cerr << "Error in listenForConnections: " << e.what() << std::endl;
        throw;
    }
}

void Server::registerClient(std::shared_ptr<tcp::socket> clientSocket){
    try {

        auto messageOpt = readMessageFromSocket(*clientSocket);
        if (!messageOpt) {
            throw std::runtime_error("Failed to read message from socket");
        }
        auto message = *messageOpt;

        // TODO: Store IP address of connection in database using LogManager!

        int clientID = message->getFromUserID();
        MessageType messageType = message->getMessageType();

        if(messageType == MessageType::CONNECT) {
            #ifdef _DEBUG
                std::cout << "Registering CONNECT message" << std::endl;
            #endif

            MessageBuilder responseBuilder;
            #ifdef _MOCK_TESTING
                responseBuilder.setMessageContents("server");
            #endif
            if(isFromClientSocketRegistered(clientID)) {
                responseBuilder.setSuccessBit(false);
                #ifdef _DEBUG
                    std::cout << "Client ID: " << clientID << " already registered" << std::endl;
                #endif
                auto response = responseBuilder.build();
                sendMessageToClient(clientID, response);
            }
            else {
                registerFromClientSocket(std::move(clientSocket), message);
            }
        }
        else if(messageType == MessageType::AUTHENTICATE) {
            #ifdef _DEBUG
                std::cout << "Registering AUTHENTICATE message" << std::endl;
            #endif
            // clientID here should be the tempID client was assigned when they connected.
            if (!isToClientSocketRegistered(clientID)) {
                addToClientSocket(clientID, clientSocket);
            }
            _userManagerBalancer->pushWork(message);
        }
        else {
            #ifdef _DEBUG
                std::cerr << "Unexpected message type in registerClient: " << MessageInterface::messageTypeToString(messageType) << std::endl;
            #endif
            MessageBuilder responseBuilder;
            #ifdef _MOCK_TESTING
                responseBuilder.setMessageContents("server");
            #endif
            responseBuilder.setSuccessBit(false);
            auto response = responseBuilder.build();
            sendMessageToClient(clientID, response);
        }
    
    } catch(const std::exception& e) {
        std::cerr << "Error in registerClient: " << e.what() << std::endl;
    }
}

void Server::registerFromClientSocket(std::shared_ptr<tcp::socket> clientSocket, std::shared_ptr<MessageInterface> message){
    MessageBuilder responseBuilder;
    int tempID = --_tempClientID;
    try{
        #ifdef _DEBUG
            std::cout << "Attempting to register FROM socket for client " << tempID << std::endl;
        #endif

        bool successCheck = addFromClientSocket(tempID, clientSocket);
        if(!successCheck) throw std::runtime_error("Failed to add FROM client socket");
        
        #ifdef _DEBUG
            std::cout << "Client ID: " << tempID << " registered FROM client socket" << std::endl;
        #endif

        responseBuilder.setSuccessBit(true);
        responseBuilder.setFromUser(message->getFromUsername(), tempID);
        responseBuilder.setToUser(message->getFromUsername(), tempID);
        responseBuilder.setMessageContents("server");
        auto response = responseBuilder.build();

        try {
            successCheck = sendConnectMessage(tempID, response);
            if(!successCheck) throw std::runtime_error("Failed to send client confirmation message");
            
            #ifdef _DEBUG
                std::cout << "Response sent to client " << tempID << " successfully" << std::endl;
            #endif
        } catch (const std::exception& e) {
            std::cerr << "Error sending response to client " << tempID << ": " << e.what() << std::endl;
            throw;
        }
    
    } catch(const std::exception& e){
        std::cerr << "Error registering client: " << e.what() << std::endl;
        responseBuilder.setSuccessBit(false);
        responseBuilder.setMessageContents("server");
        auto response = responseBuilder.build();
        std::vector<char> data = response->serialize();
        asio::write(*clientSocket, asio::buffer(data));
    }
}

void Server::handleClient(int clientID){
    try{
        std::string clientIP = getClientSocketIP(clientID);
        if(clientIP == "") throw std::runtime_error("Client ID: " + std::to_string(clientID) + " not found in client socket map [FROM]");   

        #ifdef _DEBUG
        std::cout << "Starting client handler thread for clientID: " << clientID << " IP: " << clientIP << std::endl;
        #endif

        while(true){
            try {
                auto messageOpt = readMessageFromClient(clientID);
                if(!messageOpt) throw std::runtime_error("Failed to read message from Client ID: " + std::to_string(clientID));
                auto message = *messageOpt;
                #ifdef _DEBUG
                std::cout << "Received from client " << clientID << ": ";
                message->printMessage();
                #endif

                // Route message to appropriate manager based on type
                MessageType messageType = message->getMessageType();
                switch(messageType) {
                    case MessageType::AUTHENTICATE:
                    case MessageType::LOGOUT:
                    case MessageType::ADD_USER:
                    case MessageType::CHANGE_USER_PASSWORD:
                    case MessageType::CHANGE_USER_NAME:
                    case MessageType::DELETE_USER:
                    case MessageType::SEND_MESSAGE_TO_USER:
                        #ifdef _DEBUG
                        std::cout << "Routing to UserManager: " << MessageInterface::messageTypeToString(messageType) << std::endl;
                        #endif
                        _userManagerBalancer->pushWork(message);
                        break;
                    
                    case MessageType::CREATE_CHATROOM:
                    case MessageType::DELETE_CHATROOM:
                    case MessageType::ADD_USER_TO_CHATROOM:
                    case MessageType::SEND_MESSAGE_TO_CHATROOM:
                    case MessageType::REMOVE_USER_FROM_CHATROOM:
                        #ifdef _DEBUG
                        std::cout << "Routing to ChatroomManager: " << MessageInterface::messageTypeToString(messageType) << std::endl;
                        #endif
                        _chatroomManagerBalancer->pushWork(message);
                        break;
                    
                    case MessageType::GET_TO_USER_MESSAGES:
                    case MessageType::GET_FROM_USER_MESSAGES:
                    case MessageType::GET_BETWEEN_USERS_MESSAGES:
                    case MessageType::GET_CHATROOM_MESSAGES:
                    case MessageType::GET_CHATROOM_MESSAGES_FROM_USER:
                        #ifdef _DEBUG
                        std::cout << "Routing to LogManager: " << MessageInterface::messageTypeToString(messageType) << std::endl;
                        #endif
                        _logManagerBalancer->pushWork(message);
                        break;
                    
                    default:
                        #ifdef _DEBUG
                        std::cerr << "Unknown message type received: " << MessageInterface::messageTypeToString(messageType) << std::endl;
                        #endif
                        MessageBuilder responseBuilder;
                        responseBuilder.setMessageType(MessageType::UNDEFINED);
                        responseBuilder.setSuccessBit(false);   
                        responseBuilder.setMessageContents("server");
                        auto response = responseBuilder.build();
                        sendMessageToClient(clientID, response);
                        break;
                }
            } catch (const std::exception& e) {
                #ifdef _DEBUG
                std::cerr << "Error handling message for client " << clientID << ": " << e.what() << std::endl;
                #endif
                break;
            }
        }
    }
    catch(const asio::system_error& e){
        std::cerr << "Client " << clientID << " disconnected:" << e.what() << std::endl;
    }
    catch(const std::exception& e){
        std::cerr << "Client Handler Thread Exception for " << clientID << ": " << e.what() << std::endl;
    }

    // Clean up client sockets
    removeClientSocket(clientID);

    #ifdef _DEBUG
    std::cout << "Cleaned up client: " << clientID << std::endl;
    #endif
}

// Socket operations

bool Server::sendMessageToSocket(tcp::socket& socket, std::shared_ptr<MessageInterface> message) {
    try {
        std::vector<char> data = message->serialize();
        asio::write(socket, asio::buffer(data));
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error sending message to socket: " << e.what() << std::endl;
        return false;
    }
}

bool Server::sendConnectMessage(int clientID, std::shared_ptr<MessageInterface> message) {
    std::shared_ptr<tcp::socket> fromSocket;
    {
        auto socketMutex = getClientSocketMutex(clientID);
        if (!socketMutex) {
            #ifdef _DEBUG
                std::cerr << "Client mutex not found for client ID: " << clientID << std::endl;
            #endif
            return false;
        }
        std::lock_guard<std::mutex> lock(*socketMutex.value());
        #ifdef _DEBUG
            std::cout << "Locked client socket mutex for client ID: " << clientID << std::endl;
        #endif
        auto it = _clientSocketMap.find(clientID);
        if (it == _clientSocketMap.end() || !it->second.fromSocket) {
            return false;
        }
        if (!it->second.fromSocket->is_open()) {
            return false;
        }
        fromSocket = it->second.fromSocket;
    }
    #ifdef _DEBUG
        std::cout << "Client socket mutex unlocked" << std::endl;
    #endif
    try {
        bool successCheck = sendMessageToSocket(*fromSocket, message); 
        return successCheck;
    } catch (const std::exception& e) {
        #ifdef _DEBUG
            std::cerr << "Error sending message to client " << clientID << ": " << e.what() << std::endl;
        #endif
        return false;
    }
}

std::optional<std::shared_ptr<MessageInterface>> Server::readMessageFromSocket(tcp::socket& socket) {
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

std::optional<std::shared_ptr<MessageInterface>> Server::readMessageFromClient(int clientID) {
    std::shared_ptr<tcp::socket> fromSocket;
    {
        auto socketMutex = getClientSocketMutex(clientID);
        if (!socketMutex) {
            return std::nullopt;
        }
        std::lock_guard<std::mutex> lock(*socketMutex.value());
        auto it = _clientSocketMap.find(clientID);
        if (it == _clientSocketMap.end() || !it->second.fromSocket) {
            return std::nullopt;
        }
        fromSocket = it->second.fromSocket;
    }
    return readMessageFromSocket(*fromSocket);
}

bool Server::sendMessageToClient(int clientID, std::shared_ptr<MessageInterface> message) {
    std::shared_ptr<tcp::socket> toSocket;
    {
        auto socketMutex = getClientSocketMutex(clientID);
        if (!socketMutex) {
            return false;
        }
        std::lock_guard<std::mutex> lock(*socketMutex.value());
        #ifdef _DEBUG
            std::cout << "Locked client socket mutex for client ID: " << clientID << std::endl;
        #endif
        auto it = _clientSocketMap.find(clientID);
        if (it == _clientSocketMap.end() || !it->second.toSocket) {
            return false;
        }
        if (!it->second.toSocket->is_open()) {
            return false;
        }
        toSocket = it->second.toSocket;
    }
    #ifdef _DEBUG
        std::cout << "Client socket mutex unlocked" << std::endl;
    #endif
    try {
        bool successCheck = sendMessageToSocket(*toSocket, message); 
        return successCheck;
    } catch (const std::exception& e) {
        #ifdef _DEBUG
            std::cerr << "Error sending message to client " << clientID << ": " << e.what() << std::endl;
        #endif
        return false;
    }
}

bool Server::addFromClientSocket(int clientID, std::shared_ptr<tcp::socket> fromSocket) {
    std::lock_guard<std::mutex> lock(_clientSocketMutexMapMutex);
    if (_clientSocketMap.find(clientID) != _clientSocketMap.end()) {
        return false;
    }
    _clientSocketMap[clientID].fromSocket = std::move(fromSocket);
    auto socketMutex = std::make_shared<std::mutex>();
    _clientSocketMutexMap[clientID] = socketMutex;
    return true;
}

bool Server::addToClientSocket(int clientID, std::shared_ptr<tcp::socket> toSocket) {
    std::lock_guard<std::mutex> lock(_clientSocketMutexMapMutex);
    #ifdef _DEBUG
        std::cout << "Attempting to add TO socket for client " << clientID << std::endl;
    #endif

    auto it = _clientSocketMap.find(clientID);
    if (it != _clientSocketMap.end() && it->second.toSocket != nullptr) {
        return false;
    }

    if (it == _clientSocketMap.end()) {
        _clientSocketMap[clientID] = ClientSockets();
        auto socketMutex = std::make_shared<std::mutex>();
        _clientSocketMutexMap[clientID] = socketMutex;
    }
    
    _clientSocketMap[clientID].toSocket = std::move(toSocket);
    return true;
}

bool Server::removeClientSocket(int clientID) {
    try {
        auto socketMutex = getClientSocketMutex(clientID);
        if (!socketMutex) {
            return false;
        }
        std::lock_guard<std::mutex> lock(*socketMutex.value());

        auto it = _clientSocketMap.find(clientID);
        if (it == _clientSocketMap.end()) {
            throw std::runtime_error("Client socket not found for client ID: " + std::to_string(clientID));
        }

        if (it->second.fromSocket && it->second.fromSocket->is_open()) {
            it->second.fromSocket->close();
        }
        if (it->second.toSocket && it->second.toSocket->is_open()) {
            it->second.toSocket->close();
        }
        _clientSocketMap.erase(it);

        removeClientSocketMutex(clientID);
        
        #ifdef _DEBUG
            std::cout << "Client ID: " << clientID << " removed from socket mapping" << std::endl;
        #endif
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error removing client socket: " << e.what() << std::endl;
    }
    return false;
}

bool Server::isFromClientSocketRegistered(int clientID) {
    auto socketMutex = getClientSocketMutex(clientID);
    if (!socketMutex) {
        return false;
    }
    std::lock_guard<std::mutex> lock(*socketMutex.value());
    bool isRegistered = (_clientSocketMap.find(clientID) != _clientSocketMap.end() && _clientSocketMap[clientID].fromSocket != nullptr);
    return isRegistered;
}

bool Server::isToClientSocketRegistered(int clientID) {
    auto socketMutex = getClientSocketMutex(clientID);
    if (!socketMutex) {
        return false;
    }
    std::lock_guard<std::mutex> lock(*socketMutex.value());
    bool isRegistered = (_clientSocketMap.find(clientID) != _clientSocketMap.end() && _clientSocketMap[clientID].toSocket != nullptr);
    return isRegistered;
}

std::string Server::getClientSocketIP(int clientID) {
    try {
        auto socketMutex = getClientSocketMutex(clientID);
        if (!socketMutex) {
            return "";
        }
        std::lock_guard<std::mutex> lock(*socketMutex.value());
        std::string ip = _clientSocketMap[clientID].fromSocket->remote_endpoint().address().to_string();
        return ip;
    } catch (const std::exception& e) {
        std::cerr << "Error getting client socket IP: " << e.what() << std::endl;
    }
    return "";
}

// Mutex Operations
std::optional<std::shared_ptr<std::mutex>> Server::getClientSocketMutex(int clientID) {
    std::lock_guard<std::mutex> lock(_clientSocketMutexMapMutex);
    auto it = _clientSocketMutexMap.find(clientID);
    if (it != _clientSocketMutexMap.end()) {
        return it->second;
    }
    return std::nullopt;
}

bool Server::removeClientSocketMutex(int clientID) {
    std::lock_guard<std::mutex> lock(_clientSocketMutexMapMutex);
    return _clientSocketMutexMap.erase(clientID) > 0;
}

bool Server::addClientSocketMutex(int clientID, std::shared_ptr<std::mutex> socketMutex) {
    std::lock_guard<std::mutex> lock(_clientSocketMutexMapMutex);
    if (_clientSocketMutexMap.find(clientID) != _clientSocketMutexMap.end()) {
        return false;
    }
    _clientSocketMutexMap[clientID] = socketMutex;
    return true;
}

void Server::addMessageToLogBalancer(std::shared_ptr<MessageInterface> message) {
    if (_logManagerBalancer) {
        _logManagerBalancer->pushWork(message);
    }
}