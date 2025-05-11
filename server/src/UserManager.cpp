#include "UserManager.hpp"
#include "Server.hpp"

template<typename WorkType>
std::unordered_map<int, User> UserManager<WorkType>::_userMap;

template<typename WorkType>
std::unordered_map<int, std::string> UserManager<WorkType>::_userIDToName;

template<typename WorkType>
std::unordered_map<std::string, int> UserManager<WorkType>::_usernameToID;

template<typename WorkType>
void UserManager<WorkType>::setUpDatabaseConnection() {
    // Set up connection to database & set up maps!
}

template<typename WorkType>
void UserManager<WorkType>::processWork(std::pair<WorkType, std::shared_ptr<tcp::socket>>& work) {
    #ifdef _DEBUG
    std::cout << "UserManager processing message type: " << Message::messageTypeToString(work.first.getMessageType()) << std::endl;
    #endif
            
    try {
        switch(work.first.getMessageType()){
            case MessageType::SEND:
                #ifdef _DEBUG
                std::cout << "Processing SEND message" << std::endl;
                #endif
                authUser(work);
                break;
            case MessageType::LOGOUT:
                #ifdef _DEBUG
                std::cout << "Processing LOGOUT message" << std::endl;
                #endif
                logoutUser(work);
                break;
            case MessageType::ADD_USER:
                #ifdef _DEBUG
                std::cout << "Processing ADD_USER message" << std::endl;
                #endif
                addUser(work);
                break;
            case MessageType::CHANGE_USER_PASSWORD:
                #ifdef _DEBUG
                std::cout << "Processing CHANGE_USER_PASSWORD message" << std::endl;
                #endif
                changeUserPassword(work);
                break;
            case MessageType::CHANGE_USER_NAME:
                #ifdef _DEBUG
                std::cout << "Processing CHANGE_USER_NAME message" << std::endl;
                #endif
                changeUserName(work);
                break;
            case MessageType::DELETE_USER:
                #ifdef _DEBUG
                std::cout << "Processing DELETE_USER message" << std::endl;
                #endif
                deleteUser(work);
                break;
            case MessageType::SEND_MESSAGE_TO_USER:
                #ifdef _DEBUG
                std::cout << "Processing SEND_MESSAGE_TO_USER message" << std::endl;
                #endif
                sendMessageToUser(work);
                break;
            default:
                #ifdef _DEBUG
                std::cout << "Unknown message type: " << Message::messageTypeToString(work.first.getMessageType()) << std::endl;
                #endif
                break;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error in processWork: " << e.what() << std::endl;
        throw;
    }
}      

template<typename WorkType>
void UserManager<WorkType>::authUser(std::pair<WorkType, std::shared_ptr<tcp::socket>>& work){
    std::cout << "UserManager: AUTH_USER called" << std::endl;
    
    try {
        this->_server.addFromClientSocket(work.first.getFromUserID(), work.second);
        std::cout << "UserManager: Socket registered successfully" << std::endl;
        
        MessageBuilder<WorkType> responseBuilder(&work.first);
        #ifndef _MOCK_TESTING
        responseBuilder.setMessageContents("authUser");
        #endif
        WorkType responseMessage(&responseBuilder);
        
        
        std::cout << "UserManager: Serializing authUser response..." << std::endl;
        std::vector<char> responseData = responseMessage.serialize();
        std::cout << "UserManager: Sending authUser response..." << std::endl;
        asio::write(*work.second, asio::buffer(responseData));
        std::cout << "UserManager: Sent authUser response successfully" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error in authUser: " << e.what() << std::endl;
        throw;
    }
}

template<typename WorkType>
void UserManager<WorkType>::logoutUser(std::pair<WorkType, std::shared_ptr<tcp::socket>>& work){
    std::cout << "UserManager: LOGOUT called" << std::endl;
    try {
        MessageBuilder<WorkType> responseBuilder(&work.first);
        #ifndef _MOCK_TESTING
        responseBuilder.setMessageContents("logoutUser");
        #endif
        WorkType responseMessage(&responseBuilder);
        
        std::cout << "UserManager: Serializing logoutUser response..." << std::endl;
        std::vector<char> responseData = responseMessage.serialize();
        std::cout << "UserManager: Sending logoutUser response..." << std::endl;
        asio::write(*work.second, asio::buffer(responseData));
        std::cout << "UserManager: Sent logoutUser response successfully" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error in logoutUser: " << e.what() << std::endl;
        throw;
    }
}

template<typename WorkType>
void UserManager<WorkType>::addUser(std::pair<WorkType, std::shared_ptr<tcp::socket>>& work){
    std::cout << "UserManager: ADD_USER called" << std::endl;
    try {
        MessageBuilder<WorkType> responseBuilder(&work.first);
        #ifndef _MOCK_TESTING
        responseBuilder.setMessageContents("addUser");
        #endif
        WorkType responseMessage(&responseBuilder);
        
        std::cout << "UserManager: Serializing addUser response..." << std::endl;
        std::vector<char> responseData = responseMessage.serialize();
        std::cout << "UserManager: Sending addUser response..." << std::endl;
        asio::write(*work.second, asio::buffer(responseData));
        std::cout << "UserManager: Sent addUser response successfully" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error in addUser: " << e.what() << std::endl;
        throw;
    }
}

template<typename WorkType>
void UserManager<WorkType>::changeUserPassword(std::pair<WorkType, std::shared_ptr<tcp::socket>>& work){
    std::cout << "UserManager: CHANGE_USER_PASSWORD called" << std::endl;
    try {
        MessageBuilder<WorkType> responseBuilder(&work.first);
        #ifndef _MOCK_TESTING
        responseBuilder.setMessageContents("changeUserPassword");
        #endif
        WorkType responseMessage(&responseBuilder);
        
        std::cout << "UserManager: Serializing changeUserPassword response..." << std::endl;
        std::vector<char> responseData = responseMessage.serialize();
        std::cout << "UserManager: Sending changeUserPassword response..." << std::endl;
        asio::write(*work.second, asio::buffer(responseData));
        std::cout << "UserManager: Sent changeUserPassword response successfully" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error in changeUserPassword: " << e.what() << std::endl;
        throw;
    }
}

template<typename WorkType>
void UserManager<WorkType>::changeUserName(std::pair<WorkType, std::shared_ptr<tcp::socket>>& work){
    std::cout << "UserManager: CHANGE_USER_NAME called" << std::endl;
    try {
        MessageBuilder<WorkType> responseBuilder(&work.first);
        #ifndef _MOCK_TESTING
        responseBuilder.setMessageContents("changeUserName");
        #endif
        WorkType responseMessage(&responseBuilder);
        
        std::cout << "UserManager: Serializing changeUserName response..." << std::endl;
        std::vector<char> responseData = responseMessage.serialize();
        std::cout << "UserManager: Sending changeUserName response..." << std::endl;
        asio::write(*work.second, asio::buffer(responseData));
        std::cout << "UserManager: Sent changeUserName response successfully" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error in changeUserName: " << e.what() << std::endl;
        throw;
    }
}

template<typename WorkType>
void UserManager<WorkType>::deleteUser(std::pair<WorkType, std::shared_ptr<tcp::socket>>& work){
    std::cout << "UserManager: DELETE_USER called" << std::endl;
    try {
        MessageBuilder<WorkType> responseBuilder(&work.first);
        #ifndef _MOCK_TESTING
        responseBuilder.setMessageContents("deleteUser");
        #endif
        WorkType responseMessage(&responseBuilder);
        
        std::cout << "UserManager: Serializing deleteUser response..." << std::endl;
        std::vector<char> responseData = responseMessage.serialize();
        std::cout << "UserManager: Sending deleteUser response..." << std::endl;
        asio::write(*work.second, asio::buffer(responseData));
        std::cout << "UserManager: Sent deleteUser response successfully" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error in deleteUser: " << e.what() << std::endl;
        throw;
    }
}

template<typename WorkType>
void UserManager<WorkType>::sendMessageToUser(std::pair<WorkType, std::shared_ptr<tcp::socket>>& work){
    std::cout << "UserManager: SEND_MESSAGE_TO_USER called" << std::endl;
    try {
        MessageBuilder<WorkType> responseBuilder(&work.first);
        #ifndef _MOCK_TESTING
        responseBuilder.setMessageContents("sendMessageToUser");
        #endif
        WorkType responseMessage(&responseBuilder);
        
        std::cout << "UserManager: Serializing sendMessageToUser response..." << std::endl;
        std::vector<char> responseData = responseMessage.serialize();
        std::cout << "UserManager: Sending sendMessageToUser response..." << std::endl;
        asio::write(*work.second, asio::buffer(responseData));
        std::cout << "UserManager: Sent sendMessageToUser response successfully" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error in sendMessageToUser: " << e.what() << std::endl;
        throw;
    }
}

template class UserManager<Message>;
template class UserManager<MockMessage>;