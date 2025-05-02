#ifndef SERVER_HPP
#define SERVER_HPP

#include <atomic>
#include <thread>
#include <iostream>
#include "server_include/UserManagerBalancer.hpp"
#include "server_include/ChatroomManagerBalancer.hpp"
#include "server_include/LogManagerBalancer.hpp"
#include "Message.hpp"

class Server : public std::enable_shared_from_this<Server> {
    private:
        const int _numUserManagers = 2;
        const int _numChatroomManagers = 2;
        const int _numLogManagers = 2;
        std::atomic<bool> _isRunning = true;
        int _port;
        std::string _serverIP;
        // MAP TO USERID: SOCKET ... Need to create!
        std::unique_ptr<UserManagerBalancer> _userManagerBalancer;
        std::unique_ptr<ChatroomManagerBalancer> _chatroomManagerBalancer;
        //std::unique_ptr<LogManagerBalancer> _logManagerBalancer = std::make_unique<LogManagerBalancer>(_numLogManagers, std::weak_ptr<Server>(shared_from_this()));

        std::unordered_map<int, std::thread> userManagerThreads;
        std::unordered_map<int, std::thread> chatroomManagerThreads;
        //std::unordered_map<int, std::thread> logManagerThreads;

    private:
        void initBalancers(){
            _userManagerBalancer = std::make_unique<UserManagerBalancer>(_numUserManagers, std::weak_ptr<Server>(shared_from_this()));
            _chatroomManagerBalancer = std::make_unique<ChatroomManagerBalancer>(_numChatroomManagers, std::weak_ptr<Server>(shared_from_this()));
        }
    public:
        void run(int port){
            startServer(port);
            for(int i = 0 ; i < 3; ++i){
                std::this_thread::sleep_for(std::chrono::seconds(2));
            }
            stopServer();
        }
        void startServer(int port){
            initBalancers();
            std::thread(listenForConnections);
            std::cout << "Server Started\n";
        }

        void stopServer(){
            _isRunning = false;
            std::cout << "Server Stopped!\n";
        }

        bool IsRunning() const {return _isRunning;}

        void addMessageToUserBalancer(Message& message);
        void addMessageToChatroomBalancer(Message& message);

        // Open Socket, temporarily store w/ userID = -1
        // Then handleLoginRequest, which sends the socket & the Message received to the UserManagerBalancer
        // UserManagerBalancer then handles authentication... Need a way to signal back to server that the user was successfully logged in
        // Once successfuly signal sent, then needs to send a signal to every relevant worker thread that to update cached value (if it has) of that user/chatroom
        // Then a new thread is created running processResponse() for that socket
        void listenForConnections() {while(_isRunning){};}

        void startChatroomManagers(int);
        void startLogManagers(int);
        void startUserManagers(int);
        void handleLoginRequest();
        void processResponse(); // PUT IN A TRY CATCH
};

#endif