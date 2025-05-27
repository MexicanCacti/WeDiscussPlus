#include "UserManager.hpp"
#include "Server.hpp"
#include <iostream>

std::unordered_map<std::string, int> UserManager::_usernameToUserID;
std::unordered_map<int, std::string> UserManager::_userIDToUsername;
std::shared_mutex UserManager::_userLookupMutex;
std::unordered_map<int, bool> UserManager::_clientHandlerStarted;

void UserManager::setUpDatabaseConnection() {
    
}

void UserManager::processWork(std::shared_ptr<MessageInterface>& work) {
    #ifdef _DEBUG
        std::cout << "UserManager processing message type: " << MessageInterface::messageTypeToString(work->getMessageType()) << std::endl;
    #endif
    try {
        switch(work->getMessageType()) {
            case MessageType::AUTHENTICATE:
                authUser(work);
                break;
            case MessageType::LOGOUT:
                logoutUser(work);
                break;
            case MessageType::ADD_USER:
                addUser(work);
                break;
            case MessageType::CHANGE_USER_PASSWORD:
                changeUserPassword(work);
                break;
            case MessageType::CHANGE_USER_NAME:
                changeUserName(work);
                break;
            case MessageType::DELETE_USER:
                deleteUser(work);
                break;
            case MessageType::SEND_MESSAGE_TO_USER:
                sendMessageToUser(work);
                break;
            default:
                break;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error processing work: " << e.what() << std::endl;
    }
}

void UserManager::authUser(std::shared_ptr<MessageInterface>& work){
    #ifdef _MOCK_TESTING
        sendRoutingTestMessage(work, "authUser");
        return;
    #endif

    MessageBuilder responseBuilder;
    int tempID = work->getFromUserID();
    try {
        bool areSocketsRegistered = (this->_server.isToClientSocketRegistered(tempID) && this->_server.isFromClientSocketRegistered(tempID));
        if(!areSocketsRegistered) {
            std::cerr << "Socket registration check failed for client " << tempID << std::endl;
            throw std::runtime_error("Socket(s) not registered");
        }
        
        int userID = tempID;

        // TODO: Authenticate user, add init data to message (tack on UserObject, etc.)
        // 1. Get real clientID from authentication
        // 2. Start client handler thread with real clientID
        // 3. Move sockets from tempID to clientID mapping
        // 4. Send response to client with real clientID
        // 5. Remove tempID mapping

        // Start client handler thread before any response handling
        // Only start the thread if it hasn't been started yet
        if (_clientHandlerStarted.find(userID) == _clientHandlerStarted.end()) {
            std::thread clientThread([this, userID](){
                this->_server.handleClient(userID);
            });
            clientThread.detach();
            _clientHandlerStarted[userID] = true;
        }

        responseBuilder.setSuccessBit(true);
        responseBuilder.setMessageContents("authUser");
        responseBuilder.setFromUser(work->getFromUsername(), userID);
        auto responseMessage = responseBuilder.build();
        this->_server.sendMessageToClient(userID, responseMessage);
        
        // Only remove tempID mapping after sockets are moved to real clientID
        // this->_server.removeClientSocket(tempID);  // Commented out until socket migration is implemented
    } catch (const std::exception& e) {
        std::cerr << "Error in authUser: " << e.what() << std::endl;
        responseBuilder.setSuccessBit(false);
        responseBuilder.setMessageContents("authUser");
        responseBuilder.setFromUser(work->getFromUsername(), tempID);
        auto failedMessage = responseBuilder.build();
        this->_server.sendMessageToClient(tempID, failedMessage);
        // Only remove socket after sending error response
        this->_server.removeClientSocket(tempID);
        throw std::runtime_error("Error in authUser: " + std::string(e.what()));
    }
}

void UserManager::logoutUser(std::shared_ptr<MessageInterface>& work){
    #ifdef _MOCK_TESTING
        sendRoutingTestMessage(work, "logoutUser");
        return;
    #endif

    MessageBuilder responseBuilder;
    int userID = work->getFromUserID();
    try {
        // TODO: Implement logout logic
        responseBuilder.setMessageContents("logoutUser");
        responseBuilder.setSuccessBit(true);
        auto responseMessage = responseBuilder.build();
        this->_server.sendMessageToClient(userID, responseMessage);
        this->_server.removeClientSocket(userID);
        _clientHandlerStarted.erase(userID);
    } catch (const std::exception& e) {
        std::cerr << "Error in logoutUser: " << e.what() << std::endl;
        responseBuilder.setSuccessBit(false);
        responseBuilder.setMessageContents("logoutUser");
        responseBuilder.setFromUser(work->getFromUsername(), userID);
        auto failedMessage = responseBuilder.build();
        this->_server.sendMessageToClient(userID, failedMessage);

        this->_server.removeClientSocket(userID);
        // Clean up client handler even if there's an error
        _clientHandlerStarted.erase(userID);
    }
}

void UserManager::addUser(std::shared_ptr<MessageInterface>& work){
    #ifdef _MOCK_TESTING
        sendRoutingTestMessage(work, "addUser");
        return;
    #endif

    MessageBuilder responseBuilder;
    int userID = work->getFromUserID();
    try {

        std::string username = work->getToUsername();
        std::string password = work->getMessageContents(); 

        // TODO: Get userID from database
        int newUserID = 0; 

        {
            std::unique_lock<std::shared_mutex> lookupLock(_userLookupMutex);
            
            _usernameToUserID[username] = newUserID;
            _userIDToUsername[newUserID] = username;
        }
        
        responseBuilder.setSuccessBit(true);
        responseBuilder.setMessageContents("addUser");
        auto responseMessage = responseBuilder.build();
        this->_server.sendMessageToClient(userID, responseMessage);
    } catch (const std::exception& e) {
        std::cerr << "Error in addUser: " << e.what() << std::endl;
        responseBuilder.setSuccessBit(false);
        responseBuilder.setMessageContents("addUser");
        auto failedMessage = responseBuilder.build();
        this->_server.sendMessageToClient(userID, failedMessage);
        throw std::runtime_error("Error in addUser: " + std::string(e.what()));
    }
}

void UserManager::changeUserPassword(std::shared_ptr<MessageInterface>& work){
    #ifdef _MOCK_TESTING
        sendRoutingTestMessage(work, "changeUserPassword");
        return;
    #endif

    MessageBuilder responseBuilder;
    int userID = work->getFromUserID();
    try {
        // TODO: Implement changeUserPassword logic
        responseBuilder.setSuccessBit(true);
        responseBuilder.setMessageContents("changeUserPassword");
        auto responseMessage = responseBuilder.build();
        this->_server.sendMessageToClient(userID, responseMessage);
    } catch (const std::exception& e) {
        std::cerr << "Error in changeUserPassword: " << e.what() << std::endl;
        responseBuilder.setSuccessBit(false);
        responseBuilder.setMessageContents("changeUserPassword");
        auto failedMessage = responseBuilder.build();
        this->_server.sendMessageToClient(userID, failedMessage);
        throw std::runtime_error("Error in changeUserPassword: " + std::string(e.what()));
    }
}

void UserManager::changeUserName(std::shared_ptr<MessageInterface>& work){
    #ifdef _MOCK_TESTING
        sendRoutingTestMessage(work, "changeUserName");
        return;
    #endif

    MessageBuilder responseBuilder;
    int userID = work->getFromUserID();
    try {
        // TODO: Implement changeUserName logic
        responseBuilder.setSuccessBit(true);
        responseBuilder.setMessageContents("changeUserName");
        auto responseMessage = responseBuilder.build();
        this->_server.sendMessageToClient(userID, responseMessage);
    } catch (const std::exception& e) {
        std::cerr << "Error in changeUserName: " << e.what() << std::endl;
        responseBuilder.setSuccessBit(false);
        responseBuilder.setMessageContents("changeUserName");
        auto failedMessage = responseBuilder.build();
        this->_server.sendMessageToClient(userID, failedMessage);
        throw std::runtime_error("Error in changeUserName: " + std::string(e.what()));
    }
}

void UserManager::deleteUser(std::shared_ptr<MessageInterface>& work){
    #ifdef _MOCK_TESTING
        sendRoutingTestMessage(work, "deleteUser");
        return;
    #endif

    MessageBuilder responseBuilder;
    int userID = work->getFromUserID();
    try {
        // TODO: Implement deleteUser logic... Remove from database, force logout if necessary.
        responseBuilder.setSuccessBit(true);
        responseBuilder.setMessageContents("deleteUser");
        auto responseMessage = responseBuilder.build();
        this->_server.sendMessageToClient(userID, responseMessage);
    } catch (const std::exception& e) {
        std::cerr << "Error in deleteUser: " << e.what() << std::endl;
        responseBuilder.setSuccessBit(false);
        responseBuilder.setMessageContents("deleteUser");
        auto failedMessage = responseBuilder.build();
        this->_server.sendMessageToClient(userID, failedMessage);
        throw std::runtime_error("Error in deleteUser: " + std::string(e.what()));
    }
}

void UserManager::sendMessageToUser(std::shared_ptr<MessageInterface>& work){
    #ifdef _MOCK_TESTING
        sendRoutingTestMessage(work, "sendMessageToUser");
        return;
    #endif

    MessageBuilder responseBuilder;
    int userID = work->getFromUserID();
    try {
        // TODO: Implement sendMessageToUser logic
        responseBuilder.setSuccessBit(true);
        responseBuilder.setMessageContents("sendMessageToUser");
        auto responseMessage = responseBuilder.build();
        this->_server.sendMessageToClient(userID, responseMessage);   
    } catch (const std::exception& e) {
        std::cerr << "Error in sendMessageToUser: " << e.what() << std::endl;
        responseBuilder.setSuccessBit(false);
        responseBuilder.setMessageContents("sendMessageToUser");
        auto failedMessage = responseBuilder.build();
        this->_server.sendMessageToClient(userID, failedMessage);
        throw std::runtime_error("Error in sendMessageToUser: " + std::string(e.what()));
    }
}