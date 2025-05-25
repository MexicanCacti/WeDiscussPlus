#include "UserManager.hpp"
#include "Server.hpp"
#include <iostream>

template<typename WorkType>
void UserManager<WorkType>::setUpDatabaseConnection() {
    try {
        const std::string dbPath = "wediscuss.db";
        if (!this->initializeDatabase(dbPath)) {
            throw std::runtime_error("Failed to initialize database connection");
        }
        
        #ifdef _DEBUG
            std::cout << "Database connection established for UserManager" << std::endl;
        #endif
    } catch (const std::exception& e) {
        std::cerr << "Error in setUpDatabaseConnection: " << e.what() << std::endl;
        throw;
    }
}

template<typename WorkType>
void UserManager<WorkType>::processWork(WorkType& work) {
    try {
        switch(work.getMessageType()) {
            case MessageType::AUTHENTICATE:
                #ifdef _DEBUG
                    std::cout << "Processing authenticate work" << std::endl;
                #endif
                authUser(work);
                break;
            case MessageType::LOGOUT:
                #ifdef _DEBUG
                    std::cout << "Processing logout work" << std::endl;
                #endif
                logoutUser(work);
                break;
            case MessageType::ADD_USER:
                #ifdef _DEBUG
                    std::cout << "Processing add user work" << std::endl;
                #endif
                addUser(work);
                break;
            case MessageType::CHANGE_USER_PASSWORD:
                #ifdef _DEBUG
                    std::cout << "Processing change user password work" << std::endl;
                #endif
                changeUserPassword(work);
                break;
            case MessageType::CHANGE_USER_NAME:
                #ifdef _DEBUG
                    std::cout << "Processing change user name work" << std::endl;
                #endif
                changeUserName(work);
                break;
            case MessageType::DELETE_USER:
                #ifdef _DEBUG
                    std::cout << "Processing delete user work" << std::endl;
                #endif
                deleteUser(work);
                break;
            case MessageType::SEND_MESSAGE_TO_USER:
                #ifdef _DEBUG
                    std::cout << "Processing send message to user work" << std::endl;
                #endif
                sendMessageToUser(work);
                break;
            default:
                #ifdef _DEBUG
                    std::cerr << "Unknown message type: " << Message::messageTypeToString(work.getMessageType()) << std::endl;
                #endif
                break;
        }
    }
    catch(const std::exception& e) {
        std::cerr << "Error processing work: " << e.what() << std::endl;
    }
}

template<typename WorkType>
void UserManager<WorkType>::authUser(WorkType& work){
    MessageBuilder<WorkType> responseBuilder(&work);
    int tempID = work.getFromUserID();
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
        
        #ifdef _MOCK_TESTING
            responseBuilder.setMessageContents("authUser");
            responseBuilder.setSuccessBit(true);
            responseBuilder.setFromUserID(tempID); 
            WorkType mockMessage(&responseBuilder);
            std::cout << "Sending mock message to client " << tempID << std::endl;
            this->_server.sendMessageToClient(tempID, mockMessage);
            // Don't remove socket in mock testing, tempID is the real clientID
            return;
        #endif

        responseBuilder.setSuccessBit(true);
        responseBuilder.setMessageContents("authUser");
        responseBuilder.setFromUserID(userID);  // Set the fromUserID to match the tempID
        WorkType responseMessage(&responseBuilder);
        this->_server.sendMessageToClient(userID, responseMessage);
        
        // Only remove tempID mapping after sockets are moved to real clientID
        // this->_server.removeClientSocket(tempID);  // Commented out until socket migration is implemented
    } catch (const std::exception& e) {
        std::cerr << "Error in authUser: " << e.what() << std::endl;
        try {
            responseBuilder.setSuccessBit(false);
            responseBuilder.setMessageContents("authUser");
            responseBuilder.setFromUserID(tempID);  // Set the fromUserID to match the tempID
            WorkType failedMessage(&responseBuilder);
            this->_server.sendMessageToClient(tempID, failedMessage);
            // Only remove socket after sending error response
            this->_server.removeClientSocket(tempID);
        } catch (const std::exception& e2) {
            std::cerr << "Error sending error response: " << e2.what() << std::endl;
            // If we can't send the error response, still try to clean up the socket
            this->_server.removeClientSocket(tempID);
        }
        throw;
    }
}

template<typename WorkType>
void UserManager<WorkType>::logoutUser(WorkType& work){
    #ifdef _DEBUG
        std::cout << "Logging out user " << work.getFromUserID() << std::endl;
    #endif
    MessageBuilder<WorkType> responseBuilder(&work);
    int userID = work.getFromUserID();
    try {
        #ifdef _MOCK_TESTING
            std::cout << "Entering mock testing block for user " << userID << std::endl;
            responseBuilder.setMessageContents("logoutUser");
            responseBuilder.setSuccessBit(true);
            responseBuilder.setFromUserID(userID);
            WorkType mockMessage(&responseBuilder);
            #ifdef _DEBUG
                std::cout << "Sending mock logout response to client " << userID << std::endl;
            #endif
            this->_server.sendMessageToClient(userID, mockMessage);
            #ifdef _DEBUG
                std::cout << "Mock logout response sent successfully" << std::endl;
            #endif
            return;
        #endif
        // TODO: Implement logout logic
        responseBuilder.setMessageContents("logoutUser");
        responseBuilder.setSuccessBit(true);
        WorkType responseMessage(&responseBuilder);
        this->_server.sendMessageToClient(userID, responseMessage);
        this->_server.removeClientSocket(userID);
        _clientHandlerStarted.erase(userID);
    } catch (const std::exception& e) {
        std::cerr << "Error in logoutUser: " << e.what() << std::endl;
        try {
            responseBuilder.setSuccessBit(false);
            responseBuilder.setMessageContents("logoutUser");
            WorkType failedMessage(&responseBuilder);
            this->_server.sendMessageToClient(userID, failedMessage);
        } catch (const std::exception& e2) {
            std::cerr << "Error sending error response: " << e2.what() << std::endl;
        }
        this->_server.removeClientSocket(userID);
        // Clean up client handler even if there's an error
        _clientHandlerStarted.erase(userID);
    }
}

template<typename WorkType>
void UserManager<WorkType>::addUser(WorkType& work) {
    MessageBuilder<WorkType> responseBuilder(&work);
    int userID = work.getFromUserID();
    try {
        #ifdef _MOCK_TESTING
            responseBuilder.setMessageContents("addUser");
            responseBuilder.setSuccessBit(true);
            WorkType mockMessage(&responseBuilder);
            this->_server.sendMessageToClient(userID, mockMessage);
            return;
        #endif

        std::string username = work.getToUsername();
        std::string password = work.getMessageContents(); 

        if (!this->getDatabase().insertUser(username, password)) {
            throw std::runtime_error("Failed to insert user into database");
        }

        {
            std::unique_lock<std::shared_mutex> mapLock(_userMapMutex);
            std::unique_lock<std::shared_mutex> lookupLock(_userLookupMutex);
            
            int newUserID = this->getDatabase().getUserID(username);
            
            _userMap[newUserID] = User(username, password);
            _usernameToUserID[username] = newUserID;
            _userIDToUsername[newUserID] = username;
        }
        
        responseBuilder.setSuccessBit(true);
        responseBuilder.setMessageContents("addUser");
        WorkType responseMessage(&responseBuilder);
        this->_server.sendMessageToClient(userID, responseMessage);
    } catch (const std::exception& e) {
        std::cerr << "Error in addUser: " << e.what() << std::endl;
        responseBuilder.setSuccessBit(false);
        responseBuilder.setMessageContents("addUser");
        WorkType failedMessage(&responseBuilder);
        this->_server.sendMessageToClient(userID, failedMessage);
        throw;
    }
}

template<typename WorkType>
void UserManager<WorkType>::changeUserPassword(WorkType& work){
    MessageBuilder<WorkType> responseBuilder(&work);
    int userID = work.getFromUserID();
    try {
        #ifdef _MOCK_TESTING
            responseBuilder.setMessageContents("changeUserPassword");
            responseBuilder.setSuccessBit(true);
            WorkType mockMessage(&responseBuilder);
            this->_server.sendMessageToClient(userID, mockMessage);
            return;
        #endif
        // TODO: Implement changeUserPassword logic
        responseBuilder.setSuccessBit(true);
        responseBuilder.setMessageContents("changeUserPassword");
        WorkType responseMessage(&responseBuilder);
        this->_server.sendMessageToClient(userID, responseMessage);
    } catch (const std::exception& e) {
        std::cerr << "Error in changeUserPassword: " << e.what() << std::endl;
        MessageBuilder<WorkType> responseBuilder(&work);
        responseBuilder.setSuccessBit(false);
        responseBuilder.setMessageContents("changeUserPassword");
        WorkType failedMessage(&responseBuilder);
        this->_server.sendMessageToClient(userID, failedMessage);
        throw;
    }
}

template<typename WorkType>
void UserManager<WorkType>::changeUserName(WorkType& work){
    MessageBuilder<WorkType> responseBuilder(&work);
    int userID = work.getFromUserID();
    try {
        #ifdef _MOCK_TESTING
            responseBuilder.setMessageContents("changeUserName");
            responseBuilder.setSuccessBit(true);
            WorkType mockMessage(&responseBuilder);
            this->_server.sendMessageToClient(userID, mockMessage);
            return;
        #endif
        // TODO: Implement changeUserName logic
        responseBuilder.setSuccessBit(true);
        responseBuilder.setMessageContents("changeUserName");
        WorkType responseMessage(&responseBuilder);
        this->_server.sendMessageToClient(userID, responseMessage);
    } catch (const std::exception& e) {
        std::cerr << "Error in changeUserName: " << e.what() << std::endl;
        MessageBuilder<WorkType> responseBuilder(&work);
        responseBuilder.setSuccessBit(false);
        responseBuilder.setMessageContents("changeUserName");
        WorkType failedMessage(&responseBuilder);
        this->_server.sendMessageToClient(userID, failedMessage);
        throw;
    }
}

template<typename WorkType>
void UserManager<WorkType>::deleteUser(WorkType& work){
    MessageBuilder<WorkType> responseBuilder(&work);
    int userID = work.getFromUserID();
    try {
        #ifdef _MOCK_TESTING
            responseBuilder.setMessageContents("deleteUser");
            responseBuilder.setSuccessBit(true);
            WorkType mockMessage(&responseBuilder);
            this->_server.sendMessageToClient(userID, mockMessage);
            return;
        #endif
        // TODO: Implement deleteUser logic... Remove from database, force logout if necessary.
        responseBuilder.setSuccessBit(true);
        responseBuilder.setMessageContents("deleteUser");
        WorkType responseMessage(&responseBuilder);
        this->_server.sendMessageToClient(userID, responseMessage);
    } catch (const std::exception& e) {
        std::cerr << "Error in deleteUser: " << e.what() << std::endl;
        MessageBuilder<WorkType> responseBuilder(&work);
        responseBuilder.setSuccessBit(false);
        responseBuilder.setMessageContents("deleteUser");
        WorkType failedMessage(&responseBuilder);
        this->_server.sendMessageToClient(userID, failedMessage);
        throw;
    }
}

template<typename WorkType>
void UserManager<WorkType>::sendMessageToUser(WorkType& work){
    MessageBuilder<WorkType> responseBuilder(&work);
    int userID = work.getFromUserID();
    try {
        #ifdef _MOCK_TESTING
            responseBuilder.setMessageContents("sendMessageToUser");
            responseBuilder.setSuccessBit(true);
            WorkType mockMessage(&responseBuilder);
            this->_server.sendMessageToClient(userID, mockMessage);
            return;
        #endif
        // TODO: Implement sendMessageToUser logic
        responseBuilder.setSuccessBit(true);
        responseBuilder.setMessageContents("sendMessageToUser");
        WorkType responseMessage(&responseBuilder);
        this->_server.sendMessageToClient(userID, responseMessage);   
    } catch (const std::exception& e) {
        std::cerr << "Error in sendMessageToUser: " << e.what() << std::endl;
        responseBuilder.setSuccessBit(false);
        responseBuilder.setMessageContents("sendMessageToUser");
        WorkType failedMessage(&responseBuilder);
        this->_server.sendMessageToClient(userID, failedMessage);
        throw;
    }
}

/*
template<typename WorkType>
std::optional<std::mutex&> UserManager<WorkType>::getUserMutex(int userID) {
    std::shared_lock<std::shared_mutex> lock(_userMapMutex);
    auto it = _userMap.find(userID);
    if (it != _userMap.end()) {
        return std::ref(it->second.getMutex());
    }
    return std::nullopt;
}

template<typename WorkType>
bool UserManager<WorkType>::addUserMutex(int userID) {
    std::unique_lock<std::shared_mutex> lock(_userMapMutex);
    auto it = _userMap.find(userID);
    if (it != _userMap.end()) {
        return false;
    }
    _userMap[userID] = User();
    return true;
}

template<typename WorkType>
bool UserManager<WorkType>::removeUserMutex(int userID) {
    std::unique_lock<std::shared_mutex> lock(_userMapMutex);
    return _userMap.erase(userID) > 0;
}
*/
template class UserManager<Message>;
template class UserManager<MockMessage>;