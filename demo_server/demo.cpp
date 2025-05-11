#include <unordered_map>
#include <iostream>
#include <thread>
#include <atomic>
#include <queue>
#include "shared_include/Chatroom.hpp"
#include "shared_include/User.hpp"
#include "shared_include/MessageBuilder.hpp"
#include "shared_include/Message.hpp"
#include <asio.hpp>

using asio::ip::tcp;

class ServerTest{
    private:
        struct ClientSockets{
            std::shared_ptr<tcp::socket> fromSocket;
            std::shared_ptr<tcp::socket> toSocket;
        };

        int _port;
        std::string _serverIP = "";
        bool _isRunning = true;

        // Map to clientID: To&From Sockets
        std::unordered_map<int, ClientSockets> clientSocketMap;
        std::mutex clientSocketMapMutex;

        // Map to clientID : Thread servicing client
        std::unordered_map<int, std::thread> clientThreadMap;
        std::mutex clientThreadMapMutex;


        std::unordered_map<int, std::queue<Message>> outboundMessageQueues;
        std::unordered_map<int, std::mutex> queueMutexes;

        // Needed for ASIO networking
        asio::io_context _ioContext;

        void registerClient(std::shared_ptr<tcp::socket> clientSocket){
            try{
                std::cout << "registerClient(): Waiting for initial message...\n";
                Message message = readMessageFromSocket(*clientSocket);
                int clientID(message.getFromUserID());
                MessageType messageType = message.getMessageType();

                std::cout << "MESSAGE FROM CLIENT: ";
                message.printMessage();
                std::cout << std::endl;

                if(messageType == MessageType::RECV){
                    registerToClientSocket(clientID, std::move(clientSocket));
                }
                else if(messageType == MessageType::SEND){
                    registerFromClientSocket(clientID, std::move(clientSocket));
                }
            }
            catch(std::exception& e){
                std::cerr << "Error during socket register: " << e.what() << std::endl;
            }
        }

        void registerToClientSocket(int clientID, std::shared_ptr<tcp::socket> clientSocket){
            std::unique_lock<std::mutex> socketLock(clientSocketMapMutex);
            clientSocketMap[clientID].toSocket = clientSocket;
            socketLock.unlock();

            std::cout << "Client: " << clientID << " registered Socket TO Client\n";

            std::thread clientThread([this, clientID]() {
                this->handleClient(clientID);
            });
            
            std::unique_lock<std::mutex> threadLock(clientThreadMapMutex);
            clientThreadMap[clientID] = std::move(clientThread);
            threadLock.unlock();
            std::cout << "Client: " << clientID << " registered handler thread\n";
        }

        void registerFromClientSocket(int clientID, std::shared_ptr<tcp::socket> clientSocket){
            std::unique_lock<std::mutex> socketLock(clientSocketMapMutex);
            clientSocketMap[clientID].fromSocket = clientSocket;
            socketLock.unlock();

            std::cout << "Client: " << clientID << " registered Socket FROM Client\n";
        }

        void listenForConnections(){
            tcp::acceptor acceptor(_ioContext, tcp::endpoint(tcp::v4(), _port));
            tcp::endpoint localEndpoint = acceptor.local_endpoint();
            std::cout << "Server listening on: " << localEndpoint.address().to_string() << " Port: " << localEndpoint.port() << std::endl;
            while(_isRunning){
                auto socket = std::make_shared<tcp::socket>(_ioContext);
                acceptor.accept(*socket);
                std::thread(&ServerTest::registerClient, this, std::move(socket)).detach();
            }
        }

        void handleClient(int clientID) {
            try{   
                std::shared_ptr<tcp::socket> fromSocket;
                std::shared_ptr<tcp::socket> toSocket;

                std::unique_lock<std::mutex> socketLock(clientSocketMapMutex);
                auto it = clientSocketMap.find(clientID);

                if(it == clientSocketMap.end()){
                    std::cerr << "No client sockets found for clientID: " << clientID << std::endl;
                    socketLock.unlock();
                    return;
                }
                socketLock.unlock();

                fromSocket = it->second.fromSocket;
                toSocket = it->second.toSocket;

                //std::string clientIP = "";
                std::cout << "Starting client handler thread...\n";

                // Send SYSTEM ACK
                MessageBuilder<Message> ackBuilder;
                ackBuilder.setMessageType(MessageType::ACK);
                ackBuilder.setFromUserID(-1);
                ackBuilder.setToUserID(clientID);
                ackBuilder.setMessageContents("Handler thread registered.");
                Message ackMessage(&ackBuilder);
                sendMessageToSocket(*toSocket, &ackMessage);
                std::cout << "Sent ACK to Client\n";

                while(true){
                    Message message = readMessageFromSocket(*fromSocket);
                    std::cout << "Received from client " << clientID << ": ";
                    message.printMessage();
                    
                    MessageBuilder<Message> messageBuilder(&message);
                    messageBuilder.setMessageContents("I GOT YOUR MESSAGE!");
                    messageBuilder.setToUserID(clientID);
                    message = Message(&messageBuilder);
                    std::cout << "Sending echo back to client " << clientID << "\n";
                    sendMessageToSocket(*toSocket, &message);
                }
 
            }
            catch(const asio::system_error& e){
                std::cerr << "Client " << clientID << " disconnected: " << e.what() << std::endl;
            }
            catch(const std::exception& e){
                std::cerr << "Client Handler Thread Exception for  " << clientID << ": " << e.what() << std::endl;
            }

            // Clean up
            std::unique_lock<std::mutex> socketLock(clientSocketMapMutex);
            auto socketIterator = clientSocketMap.find(clientID);
            if(socketIterator != clientSocketMap.end()){
                if(socketIterator->second.fromSocket && socketIterator->second.fromSocket->is_open()) socketIterator->second.fromSocket->close();
                if(socketIterator->second.toSocket && socketIterator->second.toSocket->is_open()) socketIterator->second.toSocket->close();
                clientSocketMap.erase(socketIterator);
            }
            socketLock.unlock();

            std::unique_lock<std::mutex> threadLock(clientThreadMapMutex);
            auto threadIterator = clientThreadMap.find(clientID);
            if(threadIterator != clientThreadMap.end()){
                if(threadIterator->second.joinable()) threadIterator->second.detach();
                clientThreadMap.erase(threadIterator);
            }
            threadLock.unlock();

            std::cout << "Cleaned up client: " << clientID << std::endl;
        }

        Message readMessageFromSocket(tcp::socket& socket){
            int msgSize = 0;
            asio::read(socket, asio::buffer(&msgSize, sizeof(int)));
            if (msgSize <= 0 || msgSize > 10000) { 
                throw std::runtime_error("Invalid message size received: " + std::to_string(msgSize));
            }

            std::vector<char> buffer(msgSize);
            asio::read(socket, asio::buffer(buffer.data(), msgSize));

            return Message::deserialize(buffer);
        }

        void sendMessageToSocket(tcp::socket& socket, Message* message){
            std::vector<char> serializedMessage = message->serialize();
            asio::write(socket, asio::buffer(serializedMessage));
        }

        void shutdown() {
            _isRunning = false;
            {
                std::unique_lock<std::mutex> lock(clientThreadMapMutex);
                for (auto& [id, thread] : clientThreadMap) {
                    if (thread.joinable()) thread.join();
                }
            }
        }

    public:
        ServerTest(int port){
            _port = port;
            listenForConnections();            
        }

        ~ServerTest(){
            shutdown();
        }
};

int main(){
    ServerTest server = ServerTest(333);
    return 0;
}