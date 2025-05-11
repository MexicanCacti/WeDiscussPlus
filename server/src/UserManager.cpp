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
            
    switch(work.first.getMessageType()){
        case MessageType::SEND:
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
            #ifdef _DEBUG
            std::cout << "Unknown message type: " << Message::messageTypeToString(work.first.getMessageType()) << std::endl;
            #endif
            break;
    }
}      

template<typename WorkType>
void UserManager<WorkType>::authUser(std::pair<WorkType, std::shared_ptr<tcp::socket>>& work){
    #ifdef _DEBUG
    std::cout << "UserManager: AUTH_USER called" << std::endl;
    #endif
    #ifdef _MOCK_TESTING
    work.first.setMessageContents("authUser");
    asio::write(*work.second, asio::buffer(work.first.serialize()));
    #endif
}

template<typename WorkType>
void UserManager<WorkType>::logoutUser(std::pair<WorkType, std::shared_ptr<tcp::socket>>& work){
    #ifdef _DEBUG
    std::cout << "UserManager: LOGOUT called" << std::endl;
    #endif
    #ifdef _MOCK_TESTING
    work.first.setMessageContents("logoutUser");
    asio::write(*work.second, asio::buffer(work.first.serialize()));
    #endif
}

template<typename WorkType>
void UserManager<WorkType>::addUser(std::pair<WorkType, std::shared_ptr<tcp::socket>>& work){
    #ifdef _DEBUG
    std::cout << "UserManager: ADD_USER called" << std::endl;
    #endif
    #ifdef _MOCK_TESTING
    work.first.setMessageContents("addUser");
    asio::write(*work.second, asio::buffer(work.first.serialize()));
    #endif
}

template<typename WorkType>
void UserManager<WorkType>::changeUserPassword(std::pair<WorkType, std::shared_ptr<tcp::socket>>& work){
    #ifdef _DEBUG
    std::cout << "UserManager: CHANGE_USER_PASSWORD called" << std::endl;
    #endif
    #ifdef _MOCK_TESTING
    work.first.setMessageContents("changeUserPassword");
    asio::write(*work.second, asio::buffer(work.first.serialize()));
    #endif
}

template<typename WorkType>
void UserManager<WorkType>::changeUserName(std::pair<WorkType, std::shared_ptr<tcp::socket>>& work){
    #ifdef _DEBUG
    std::cout << "UserManager: CHANGE_USER_NAME called" << std::endl;
    #endif
    #ifdef _MOCK_TESTING
    work.first.setMessageContents("changeUserName");
    asio::write(*work.second, asio::buffer(work.first.serialize()));
    #endif
}

template<typename WorkType>
void UserManager<WorkType>::deleteUser(std::pair<WorkType, std::shared_ptr<tcp::socket>>& work){
    #ifdef _DEBUG
    std::cout << "UserManager: DELETE_USER called" << std::endl;
    #endif
    #ifdef _MOCK_TESTING
    work.first.setMessageContents("deleteUser");
    asio::write(*work.second, asio::buffer(work.first.serialize()));
    #endif
}

template<typename WorkType>
void UserManager<WorkType>::sendMessageToUser(std::pair<WorkType, std::shared_ptr<tcp::socket>>& work){
    #ifdef _DEBUG
    std::cout << "UserManager: SEND_MESSAGE_TO_USER called" << std::endl;
    #endif
    #ifdef _MOCK_TESTING
    work.first.setMessageContents("sendMessageToUser");
    asio::write(*work.second, asio::buffer(work.first.serialize()));
    #endif
}

template class UserManager<Message>;
template class UserManager<MockMessage>;