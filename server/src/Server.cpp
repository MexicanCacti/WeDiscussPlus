#include "Server.hpp"

template<typename WorkType>
Server<WorkType>::Server(const std::string& ip, int port) 
    : _port(port), 
      _ioContext(),
      _connectionAcceptor(_ioContext, tcp::endpoint(asio::ip::make_address(ip), port)) {}

template<typename WorkType>
void Server<WorkType>::startServer(){
    try {
        #ifdef _DEBUG
        std::cout << "Server: Starting initialization..." << std::endl;
        #endif
        
        _chatroomManagerBalancer = std::make_unique<ChatroomManagerBalancer<WorkType>>();
        _userManagerBalancer = std::make_unique<UserManagerBalancer<WorkType>>();
        _logManagerBalancer = std::make_unique<LogManagerBalancer<WorkType>>();
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

template<typename WorkType>
void Server<WorkType>::stopServer(){
    _isRunning = false;
    
    if (_chatroomManagerBalancer) _chatroomManagerBalancer->stopAllThreads();
    if (_userManagerBalancer) _userManagerBalancer->stopAllThreads();
    if (_logManagerBalancer) _logManagerBalancer->stopAllThreads();
    
    if (_connectionAcceptor.is_open()) {
        _connectionAcceptor.close();
    }
    
    _ioContext.stop();
}

template<typename WorkType>
void Server<WorkType>::addToClientSocket(int clientID, std::shared_ptr<tcp::socket> toSocket){
    std::unique_lock<std::mutex> socketLock(_clientSocketMapMutex);
    _clientSocketMap[clientID].toSocket = toSocket;
    socketLock.unlock();
}

template<typename WorkType>
void Server<WorkType>::addFromClientSocket(int clientID, std::shared_ptr<tcp::socket> fromSocket) {
    std::unique_lock<std::mutex> socketLock(_clientSocketMapMutex);
    if(_clientSocketMap.find(clientID) == _clientSocketMap.end()) {
        _clientSocketMap[clientID] = ClientSockets{nullptr, nullptr};
    }
    _clientSocketMap[clientID].fromSocket = fromSocket;
    socketLock.unlock();

    #ifdef _DEBUG
        std::cout << "Client ID: " << clientID << " registered FROM client socket" << std::endl;
    #endif
}

template<typename WorkType>
Server<WorkType>::ClientSockets Server<WorkType>::getClientSocket(int clientID) {
    std::unique_lock<std::mutex> socketLock(_clientSocketMapMutex);
    return _clientSocketMap[clientID];
}

template<typename WorkType>
void Server<WorkType>::listenForConnections(){
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
                    std::shared_ptr<tcp::socket> clientSocket = std::make_shared<tcp::socket>(_ioContext);
                    _connectionAcceptor.accept(*clientSocket);
                    #ifdef _DEBUG
                    std::cout << "Server: New connection accepted" << std::endl;
                    #endif
                    
                    std::thread([this, clientSocket]() {
                        this->registerClient(std::move(clientSocket));
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

template<typename WorkType>
void Server<WorkType>::registerClient(std::shared_ptr<tcp::socket> clientSocket){
    try {
        #ifdef _DEBUG
            std::cout << "Client connected, Waiting for initial message..." << std::endl;
        #endif

        WorkType message = readMessageFromSocket(clientSocket);
        int clientID(message.getFromUserID());
        MessageType messageType(message.getMessageType());

        #ifdef _DEBUG
            std::cout << "Client ID: " << clientID << ", Message Type: " << Message::messageTypeToString(messageType) << std::endl;
        #endif

        if(messageType == MessageType::SEND){
            #ifdef _DEBUG
                std::cout << "Registering SEND message" << std::endl;
            #endif
            _userManagerBalancer->pushWork(std::make_pair(message, clientSocket));
        }
        else if(messageType == MessageType::RECV){
            #ifdef _DEBUG
                std::cout << "Registering RECV socket" << std::endl;
            #endif
            registerToClientSocket(clientID, std::move(clientSocket));
        }
    }
    catch(const std::exception& e){
        std::cerr << "Error registering client:" << e.what() << std::endl;
    }
}

template<typename WorkType>
void Server<WorkType>::registerToClientSocket(int clientID, std::shared_ptr<tcp::socket> clientSocket){
    std::unique_lock<std::mutex> socketLock(_clientSocketMapMutex);
    if(_clientSocketMap.find(clientID) == _clientSocketMap.end()) {
        _clientSocketMap[clientID] = ClientSockets{nullptr, nullptr};
    }
    _clientSocketMap[clientID].toSocket = clientSocket;
    socketLock.unlock();

    #ifdef _DEBUG
        std::cout << "Client ID: " << clientID << " registered TO client socket" << std::endl;
    #endif
    
    std::thread clientThread([this, clientID](){
        this->handleClient(clientID);
    });
    clientThread.detach();
}

template<typename WorkType>
void Server<WorkType>::handleClient(int clientID){
    try{
        std::shared_ptr<tcp::socket> fromSocket;
        std::shared_ptr<tcp::socket> toSocket;

        {
            std::unique_lock<std::mutex> socketLock(_clientSocketMapMutex);
            auto it = _clientSocketMap.find(clientID);
            if(it == _clientSocketMap.end()){
                #ifdef _DEBUG
                std::cerr << "No client sockets found for clientID: " << clientID << std::endl;
                #endif
                return;
            }
            fromSocket = it->second.fromSocket;
            toSocket = it->second.toSocket;
        }

        if (!fromSocket || !toSocket) {
            #ifdef _DEBUG
            std::cerr << "Missing socket for clientID: " << clientID << std::endl;
            #endif
            return;
        }

        std::string clientIP = fromSocket->remote_endpoint().address().to_string();
        #ifdef _DEBUG
        std::cout << "Starting client handler thread for clientID: " << clientID << " IP: " << clientIP << std::endl;
        #endif

        // Send SYSTEM ACK
        MessageBuilder<WorkType> ackBuilder;
        ackBuilder.setMessageType(MessageType::ACK);
        ackBuilder.setFromUserID(-1);
        ackBuilder.setToUserID(clientID);
        ackBuilder.setMessageContents("Handler thread registered.");
        WorkType ackMessage(&ackBuilder);
        sendMessageToSocket(toSocket, ackMessage);
        #ifdef _DEBUG
        std::cout << "Sent ACK to Client" << std::endl;
        #endif

        while(true){
            try {
                WorkType message = readMessageFromSocket(fromSocket);
                #ifdef _DEBUG
                std::cout << "Received from client " << clientID << ": ";
                message.printMessage();
                #endif

                // Route message to appropriate manager based on type
                switch(message.getMessageType()) {
                    case MessageType::LOGOUT:
                    case MessageType::ADD_USER:
                    case MessageType::CHANGE_USER_PASSWORD:
                    case MessageType::CHANGE_USER_NAME:
                    case MessageType::DELETE_USER:
                    case MessageType::SEND_MESSAGE_TO_USER:
                    case MessageType::SEND:
                        #ifdef _DEBUG
                        std::cout << "Routing to UserManager: " << Message::messageTypeToString(message.getMessageType()) << std::endl;
                        #endif
                        _userManagerBalancer->pushWork(std::make_pair(message, toSocket));
                        break;
                    
                    case MessageType::CREATE_CHATROOM:
                    case MessageType::DELETE_CHATROOM:
                    case MessageType::ADD_USER_TO_CHATROOM:
                    case MessageType::SEND_MESSAGE_TO_CHATROOM:
                    case MessageType::REMOVE_USER_FROM_CHATROOM:
                        #ifdef _DEBUG
                        std::cout << "Routing to ChatroomManager: " << Message::messageTypeToString(message.getMessageType()) << std::endl;
                        #endif
                        _chatroomManagerBalancer->pushWork(std::make_pair(message, toSocket));
                        break;
                    
                    case MessageType::GET_USER_MESSAGES:
                    case MessageType::GET_CHATROOM_MESSAGES:
                        #ifdef _DEBUG
                        std::cout << "Routing to LogManager: " << Message::messageTypeToString(message.getMessageType()) << std::endl;
                        #endif
                        _logManagerBalancer->pushWork(std::make_pair(message, toSocket));
                        break;
                    
                    default:
                        #ifdef _DEBUG
                        std::cerr << "Unknown message type received: " << Message::messageTypeToString(message.getMessageType()) << std::endl;
                        #endif
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

    // Clean up 
    std::unique_lock<std::mutex> socketLock(_clientSocketMapMutex);
    auto socketIterator = _clientSocketMap.find(clientID);
    if(socketIterator != _clientSocketMap.end()){
        if(socketIterator->second.fromSocket && socketIterator->second.fromSocket->is_open()) socketIterator->second.fromSocket->close();
        if(socketIterator->second.toSocket && socketIterator->second.toSocket->is_open()) socketIterator->second.toSocket->close();
        _clientSocketMap.erase(socketIterator);
    }
    socketLock.unlock();

    #ifdef _DEBUG
    std::cout << "Cleaned up client: " << clientID << std::endl;
    #endif
}

template<typename WorkType>
WorkType Server<WorkType>::readMessageFromSocket(std::shared_ptr<tcp::socket> socket){
    #ifdef _DEBUG
    std::cout << "Reading message from socket" << std::endl;
    #endif
    int msgSize = 0;
    asio::read(*socket, asio::buffer(&msgSize, sizeof(int)));
    if(msgSize <= 0){
        #ifdef _DEBUG
        std::cerr << "Invalid message size received: " << msgSize << std::endl;
        #endif
        throw std::runtime_error("Invalid message size received: " + std::to_string(msgSize));
    }
    std::vector<char> buffer(msgSize);
    asio::read(*socket, asio::buffer(buffer.data(), msgSize));
    return WorkType::deserialize(buffer);
}

template<typename WorkType>
void Server<WorkType>::sendMessageToSocket(std::shared_ptr<tcp::socket> socket, WorkType& message){
    try {
        std::vector<char> serializedMessage = message.serialize();
        asio::write(*socket, asio::buffer(serializedMessage));
        #ifdef _DEBUG
        std::cout << "Message sent successfully to socket" << std::endl;
        #endif
    } catch (const std::exception& e) {
        std::cerr << "Error sending message to socket: " << e.what() << std::endl;
        throw;
    }
}

template<typename WorkType>
void Server<WorkType>::shutdown(){
    _isRunning = false;
    if(_chatroomManagerBalancer) _chatroomManagerBalancer->stopAllThreads();
    if(_userManagerBalancer) _userManagerBalancer->stopAllThreads();
    if(_logManagerBalancer) _logManagerBalancer->stopAllThreads();
}

template<typename WorkType>
void Server<WorkType>::addMessageToLogBalancer(WorkType& message){
    _logManagerBalancer->pushWork(std::make_pair(message, nullptr));
}

template class Server<Message>;
template class Server<MockMessage>;