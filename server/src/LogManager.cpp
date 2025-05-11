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
            try {
                MessageBuilder<WorkType> responseBuilder(&message.first);
                #ifndef _MOCK_TESTING
                responseBuilder.setMessageContents("getUserMessages");
                #endif
                WorkType responseMessage(&responseBuilder);
                
                std::cout << "LogManager: Serializing getUserMessages response..." << std::endl;
                std::vector<char> responseData = responseMessage.serialize();
                std::cout << "LogManager: Sending getUserMessages response..." << std::endl;
                asio::write(*message.second, asio::buffer(responseData));
                std::cout << "LogManager: Sent getUserMessages response successfully" << std::endl;
            } catch (const std::exception& e) {
                std::cerr << "Error in getUserMessages response: " << e.what() << std::endl;
                throw;
            }
            break;
        case MessageType::GET_CHATROOM_MESSAGES:
            getChatroomMessages(message.first.getToChatroomID());
            try {
                MessageBuilder<WorkType> responseBuilder(&message.first);
                #ifndef _MOCK_TESTING
                responseBuilder.setMessageContents("getChatroomMessages");
                #endif
                WorkType responseMessage(&responseBuilder);
                
                std::cout << "LogManager: Serializing getChatroomMessages response..." << std::endl;
                std::vector<char> responseData = responseMessage.serialize();
                std::cout << "LogManager: Sending getChatroomMessages response..." << std::endl;
                asio::write(*message.second, asio::buffer(responseData));
                std::cout << "LogManager: Sent getChatroomMessages response successfully" << std::endl;
            } catch (const std::exception& e) {
                std::cerr << "Error in getChatroomMessages response: " << e.what() << std::endl;
                throw;
            }
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