#include "server_include/Server.hpp"

Server::Server(int port) : _port(port), _ioContext(), _connectionAcceptor(_ioContext, tcp::endpoint(tcp::v4(), port)) {}

void Server::startServer(){
    try {
        _chatroomManagerBalancer = std::make_unique<ChatroomManagerBalancer<Message>>();
        _userManagerBalancer = std::make_unique<UserManagerBalancer<Message>>();
        _logManagerBalancer = std::make_unique<LogManagerBalancer<Message>>();
        
        _chatroomManagerBalancer->initManagers(_MAX_CHATROOM_MANAGERS, *this);
        _userManagerBalancer->initManagers(_MAX_USER_MANAGERS, *this);
        _logManagerBalancer->initManagers(_MAX_LOG_MANAGERS, *this);
        
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
    tcp::endpoint localEndpoint = _connectionAcceptor.local_endpoint();
    std::cout << "Server is listening on " << localEndpoint.address().to_string() << ":" << localEndpoint.port() << std::endl;
    _isRunning = true;
    while(_isRunning){
        try {
            std::shared_ptr<tcp::socket> clientSocket = std::make_shared<tcp::socket>(_ioContext);
            _connectionAcceptor.accept(*clientSocket);
            
            std::thread([this, clientSocket]() {
                this->registerClient(std::move(clientSocket));
            }).detach();
        } catch (const std::exception& e) {
            if (_isRunning) {
                std::cerr << "Error accepting connection: " << e.what() << std::endl;
            }
        }
    }
}

void Server::registerClient(std::shared_ptr<tcp::socket> clientSocket){
    try {
        #ifdef _DEBUG
            std::cout << "Client connected, Waiting for initial message..." << std::endl;
        #endif

        Message message = readMessageFromSocket(clientSocket);
        int clientID(message.getFromUserID());
        MessageType messageType(message.getMessageType());

        #ifdef _DEBUG
            std::cout << "Client ID: " << clientID << ", Message Type: " << Message::messageTypeToString(messageType) << std::endl;
        #endif

        if(messageType == MessageType::SEND){
            _userManagerBalancer->pushWork(std::make_pair(message, std::move(clientSocket)));
        }
        else if(messageType == MessageType::RECV){
            registerToClientSocket(clientID, std::move(clientSocket));
        }
    }
    catch(const std::exception& e){
        std::cerr << "Error registering client:" << e.what() << std::endl;
    }
}

void Server::registerToClientSocket(int clientID, std::shared_ptr<tcp::socket> clientSocket){
    /*
    std::unique_lock<std::mutex> socketLock(_clientSocketMapMutex);
    if(_clientSocketMap.find(clientID) == _clientSocketMap.end() || _clientSocketMap[clientID].fromSocket != nullptr){
        #ifdef _DEBUG
            std::cerr << "Client ID: " << clientID << " not found or not authenticated" << std::endl;
        #endif
        return;
    }
    _clientSocketMap[clientID].fromSocket = clientSocket;
    socketLock.unlock();

    #ifdef _DEBUG
        std::cout << "Client ID: " << clientID << " registered FROM client socket" << std::endl;
    #endif
    */
    std::thread clientThread([this, clientID](){
        this->handleClient(clientID);
    });
}

void Server::handleClient(int clientID){
    try{
        std::shared_ptr<tcp::socket> fromSocket;
        std::shared_ptr<tcp::socket> toSocket;

        std::unique_lock<std::mutex> socketLock(_clientSocketMapMutex);
        auto it = _clientSocketMap.find(clientID);

        if(it == _clientSocketMap.end()){
            #ifdef _DEBUG
                std::cerr << "No client sockets found for clientID: " << clientID << std::endl;
            #endif
            return;
        }
        socketLock.unlock();

        fromSocket = it->second.fromSocket;
        toSocket = it->second.toSocket;

        std::string clientIP = fromSocket->remote_endpoint().address().to_string();
        #ifdef _DEBUG
            std::cout << "Starting client handler thread for clientID: " << clientID << " IP: " << clientIP << std::endl;
        #endif

        // Send SYSTEM ACK
        MessageBuilder ackBuilder;
        ackBuilder.setMessageType(MessageType::ACK);
        ackBuilder.setFromUserID(-1);
        ackBuilder.setToUserID(clientID);
        ackBuilder.setMessageContents("Handler thread registered.");
        Message ackMessage(&ackBuilder);
        sendMessageToSocket(toSocket, ackMessage);
        #ifdef _DEBUG
            std::cout << "Sent ACK to Client" << std::endl;
        #endif

        while(true){
            Message message = readMessageFromSocket(fromSocket);
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
                    _userManagerBalancer->pushWork(std::make_pair(message, toSocket));
                    break;
                
                case MessageType::CREATE_CHATROOM:
                case MessageType::DELETE_CHATROOM:
                case MessageType::ADD_USER_TO_CHATROOM:
                case MessageType::SEND_MESSAGE_TO_CHATROOM:
                case MessageType::REMOVE_USER_FROM_CHATROOM:
                    _chatroomManagerBalancer->pushWork(std::make_pair(message, toSocket));
                    break;
                
                case MessageType::GET_USER_MESSAGES:
                case MessageType::GET_CHATROOM_MESSAGES:
                    _logManagerBalancer->pushWork(std::make_pair(message, toSocket));
                    break;
                
                default:
                    #ifdef _DEBUG
                        std::cerr << "Unknown message type received" << std::endl;
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

Message Server::readMessageFromSocket(std::shared_ptr<tcp::socket> socket){
    int msgSize = 0;
    asio::read(*socket, asio::buffer(&msgSize, sizeof(int)));
    if(msgSize <= 0){
        throw std::runtime_error("Invalid message size received: " + std::to_string(msgSize));
    }
    std::vector<char> buffer(msgSize);
    asio::read(*socket, asio::buffer(buffer.data(), msgSize));
    return Message::deserialize(buffer);
}

void Server::sendMessageToSocket(std::shared_ptr<tcp::socket> socket, Message& message){
    std::vector<char> serializedMessage = message.serialize();
    asio::write(*socket, asio::buffer(serializedMessage));
}

void Server::shutdown(){
    _isRunning = false;
    if(_chatroomManagerBalancer) _chatroomManagerBalancer->stopAllThreads();
    if(_userManagerBalancer) _userManagerBalancer->stopAllThreads();
    if(_logManagerBalancer) _logManagerBalancer->stopAllThreads();
}

void Server::addMessageToLogBalancer(Message& message){

}
