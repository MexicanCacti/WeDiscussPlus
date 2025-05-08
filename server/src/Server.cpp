#include "server_include/Server.hpp"

Server::Server(int port) : _port(port), _ioContext(), _connectionAcceptor(_ioContext, tcp::endpoint(tcp::v4(), port)) {}

void Server::startServer(){
    _chatroomManagerBalancer = std::make_unique<ChatroomManagerBalancer<Message>>();
    _userManagerBalancer = std::make_unique<UserManagerBalancer<Message>>();
    _logManagerBalancer = std::make_unique<LogManagerBalancer<Message>>();
    
    // Initialize each balancer
    _chatroomManagerBalancer->initManagers(_MAX_CHATROOM_MANAGERS, *this);
    _userManagerBalancer->initManagers(_MAX_USER_MANAGERS, *this);
    _logManagerBalancer->initManagers(_MAX_LOG_MANAGERS, *this);
    
    listenForConnections();
}

void Server::stopServer(){
    _isRunning = false;
}

void Server::listenForConnections(){
    tcp::endpoint localEndpoint = _connectionAcceptor.local_endpoint();
    std::cout << "Server is listening on " << localEndpoint.address().to_string() << ":" << localEndpoint.port() << std::endl;

    while(_isRunning){
        std::shared_ptr<tcp::socket> clientSocket = std::make_shared<tcp::socket>(_ioContext);
        _connectionAcceptor.accept(*clientSocket);
        std::thread(&Server::registerClient, this, std::move(clientSocket)).detach();
    }
}

void Server::registerClient(std::shared_ptr<tcp::socket> clientSocket){
    try{
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
            // When we get a SEND message, we know that the client wants to connect to server, we need to authenticate the client.
            // Client will send a SEND message with its username and password, we need to check if the username and password are correct
            // If Auth, will return a SEND messge with the clientID and a confirmation message.
            // On the client side, they wait for either a confirmation or a rejection, then they will send a RECV message to the server.
            // On the server side, they will store the SEND socket in a map with the clientID as the key if authentication is successful.
            // The SEND socket will be used to receive messages from the client
            registerFromClientSocket(std::move(clientSocket));
        }
        else if(messageType == MessageType::RECV){
           
            registerToClientSocket(clientID, std::move(clientSocket));
        }
    }
    catch(const std::exception& e){
        std::cerr << "Error registering client:" << e.what() << std::endl;
    }
}

void Server::registerFromClientSocket(std::shared_ptr<tcp::socket> clientSocket){
    // HERE: use USERMANAGER to authenticate the client, USERMANAGER will add the client to the map if authentication is successful.
    // USERMANAGER will also send the SEND message to the client if authentication is successful.
    // If authentication fails, USERMANAGER will send a SEND message to the client with a rejection message.
    // The client will then close the socket and try to connect again.
}

void Server::registerToClientSocket(int clientID, std::shared_ptr<tcp::socket> clientSocket){
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

            // Add message to the Load Balancer Queue!
            // TODO:
            // 1. Add message to the Load Balancer Queue
            // 2. Load Balancer will determine the message type
            // 3. Based on the message type, the Load Balancer will route the message to the appropriate handler
            // 4. The handler will process the message and send a response back to the client
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
