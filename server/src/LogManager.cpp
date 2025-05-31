#include "LogManager.hpp"
#include "Server.hpp"
#include <chrono>
using namespace std::chrono_literals;

void LogManager::processWork(std::shared_ptr<MessageInterface>& work) {
    try{
        #ifdef _DEBUG   
            std::cout << "LogManager processing message type: " << MessageInterface::messageTypeToString(work->getMessageType()) << std::endl;
        #endif
            
        switch(work->getMessageType()){
            case MessageType::GET_TO_USER_MESSAGES:
            case MessageType::GET_FROM_USER_MESSAGES:
            case MessageType::GET_BETWEEN_USERS_MESSAGES:
            case MessageType::GET_CHATROOM_MESSAGES:
            case MessageType::GET_CHATROOM_MESSAGES_FROM_USER:
                answerLogRequest(work);
                break;
            case MessageType::STORE_CONNECT_LOG:
                storeConnectLog(work);
                break;
            default:
                break;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error in processWork: " << e.what() << std::endl;
        throw;
    }
}   

void LogManager::getToUserMessages(int userID) {

}

void LogManager::getFromUserMessages(int userID) {

}
void LogManager::getBetweenUsersMessages(int userID1, int userID2) {

}
void LogManager::getChatroomMessages(int chatroomID) {

}
void LogManager::getChatroomMessagesFromUser(int chatroomID, int userID) {

}

void LogManager::answerLogRequest(std::shared_ptr<MessageInterface>& message) {
    #ifdef _MOCK_TESTING
        std::string functionCalled;
        switch(message->getMessageType()){
            case MessageType::GET_TO_USER_MESSAGES:
                functionCalled = "getToUserMessages";
                break;
            case MessageType::GET_FROM_USER_MESSAGES:
                functionCalled = "getFromUserMessages";
                break;
            case MessageType::GET_BETWEEN_USERS_MESSAGES:
                functionCalled = "getBetweenUsersMessages";
                break;
            case MessageType::GET_CHATROOM_MESSAGES:
                functionCalled = "getChatroomMessages";
                break;
            case MessageType::GET_CHATROOM_MESSAGES_FROM_USER:
                functionCalled = "getChatroomMessagesFromUser";
                break;
            default:
                return;
        }
        sendRoutingTestMessage(message, functionCalled);
        return;
    #endif

    MessageBuilder responseBuilder;
    try{
        switch(message->getMessageType()){
            case MessageType::GET_TO_USER_MESSAGES:
                getToUserMessages(message->getFromUserID());
                break;
            case MessageType::GET_FROM_USER_MESSAGES:
                getFromUserMessages(message->getFromUserID());
                break;
            case MessageType::GET_BETWEEN_USERS_MESSAGES:
                getBetweenUsersMessages(message->getFromUserID(), message->getToUserID());
                break;
            case MessageType::GET_CHATROOM_MESSAGES:
                getChatroomMessages(message->getFromUserID());
                break;
            case MessageType::GET_CHATROOM_MESSAGES_FROM_USER:
                getChatroomMessagesFromUser(message->getFromUserID(), message->getToUserID());
                break;
            default:
                break;
        }   
    } catch (const std::exception& e) {
        std::cerr << "Error in answerLogRequest: " << e.what() << std::endl;
        responseBuilder.setSuccessBit(false);
        auto responseMessage = responseBuilder.build();
        this->_server.sendMessageToClient(message->getFromUserID(), responseMessage);
        throw std::runtime_error("Error in answerLogRequest: " + std::string(e.what()));
    }
}

void LogManager::storeConnectLog(std::shared_ptr<MessageInterface>& message){
    // TODO: Implement
}