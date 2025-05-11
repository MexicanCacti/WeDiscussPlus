#include "LogManager.hpp"
#include "Server.hpp"
#include <chrono>
using namespace std::chrono_literals;

template<typename WorkType>
void LogManager<WorkType>::setUpDatabaseConnection(){
    // Set up connection to database & set up maps!
}

template<typename WorkType>
void LogManager<WorkType>::processWork(std::pair<WorkType, std::shared_ptr<tcp::socket>>& work) {
    #ifdef _DEBUG
    std::cout << "LogManager processing message type: " << WorkType::messageTypeToString(work.first.getMessageType()) << std::endl;
    #endif
            
    switch(work.first.getMessageType()){
        case MessageType::GET_USER_MESSAGES:
        case MessageType::GET_CHATROOM_MESSAGES:
            answerLogRequest(work);
            break;
        default:
            #ifdef _DEBUG
            std::cout << "Unknown message type: " << WorkType::messageTypeToString(work.first.getMessageType()) << std::endl;
            #endif
            break;
    }
}   

template<typename WorkType>
void LogManager<WorkType>::getUserMessages(int userID) {
    #ifdef _DEBUG
        std::cout << "LogManager: GET_USER_MESSAGES called for userID: " << userID << std::endl;
    #endif
}

template<typename WorkType>
void LogManager<WorkType>::getChatroomMessages(int chatroomID) {
    #ifdef _DEBUG
        std::cout << "LogManager: GET_CHATROOM_MESSAGES called for chatroomID: " << chatroomID << std::endl;
    #endif
}

template<typename WorkType>
void LogManager<WorkType>::answerLogRequest(std::pair<WorkType, std::shared_ptr<tcp::socket>>& message) {
    switch(message.first.getMessageType()){
        case MessageType::GET_USER_MESSAGES:
            getUserMessages(message.first.getToUserID());
            #ifdef _MOCK_TESTING
                message.first.setMessageContents("getUserMessages");
                asio::write(message.second, asio::buffer(message.first.serialize()));
            #endif
            break;
        case MessageType::GET_CHATROOM_MESSAGES:
            getChatroomMessages(message.first.getToChatroomID());
            #ifdef _MOCK_TESTING
                message.first.setMessageContents("getChatroomMessages");
                asio::write(message.second, asio::buffer(message.first.serialize()));
            #endif
            break;
        default:
            break;
    }
}

template<typename WorkType>
void LogManager<WorkType>::storeMessage(WorkType& message) {
    #ifdef _DEBUG
        std::cout << "LogManager: storeMessage called" << std::endl;
    #endif
}

template class LogManager<Message>;
template class LogManager<MockMessage>;