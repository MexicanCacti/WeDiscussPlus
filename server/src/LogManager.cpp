#include "LogManager.hpp"
#include "Server.hpp"
#include <chrono>
using namespace std::chrono_literals;

template<typename WorkType>
void LogManager<WorkType>::setUpDatabaseConnection(){
    // Set up connection to database & set up maps!
}

template<typename WorkType>
void LogManager<WorkType>::processWork(WorkType& work) {
    try{
        #ifdef _DEBUG   
            std::cout << "LogManager processing message type: " << WorkType::messageTypeToString(work.getMessageType()) << std::endl;
        #endif
            
        switch(work.getMessageType()){
            case MessageType::GET_USER_MESSAGES:
            case MessageType::GET_CHATROOM_MESSAGES:
                answerLogRequest(work);
                break;
            default:
                storeMessage(work);
                break;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error in processWork: " << e.what() << std::endl;
        throw;
    }
}   

template<typename WorkType>
void LogManager<WorkType>::getUserMessages(int userID) {
    try{
        // TODO: Get user messages from database
    } catch (const std::exception& e) {
        std::cerr << "Error in getUserMessages: " << e.what() << std::endl;
        throw;
    }
}

template<typename WorkType>
void LogManager<WorkType>::getChatroomMessages(int chatroomID) {
    try{
        // TODO: Get chatroom messages from database
    } catch (const std::exception& e) {
        std::cerr << "Error in getChatroomMessages: " << e.what() << std::endl;
        throw;
    }
}

template<typename WorkType>
void LogManager<WorkType>::answerLogRequest(WorkType& message) {
    MessageBuilder<WorkType> responseBuilder(&message);
    try{
        #ifdef _MOCK_TESTING
        {
            std::string responseContent;
            switch(message.getMessageType()){
                case MessageType::GET_USER_MESSAGES:
                    responseContent = "getUserMessages";
                    break;
                case MessageType::GET_CHATROOM_MESSAGES:
                    responseContent = "getChatroomMessages";
                    break;
                default:
                    return;
            }
            responseBuilder.setMessageContents(responseContent);
            responseBuilder.setSuccessBit(true);
            WorkType responseMessage(&responseBuilder);
            this->_server.sendMessageToClient(message.getFromUserID(), responseMessage);
            return;
        }
        #endif

        switch(message.getMessageType()){
            case MessageType::GET_USER_MESSAGES:
                getUserMessages(message.getFromUserID());
                break;
            case MessageType::GET_CHATROOM_MESSAGES:
                getChatroomMessages(message.getFromUserID());
                break;
            default:
                break;
        }   
    } catch (const std::exception& e) {
        std::cerr << "Error in answerLogRequest: " << e.what() << std::endl;
        responseBuilder.setSuccessBit(false);
        WorkType responseMessage(&responseBuilder);
        this->_server.sendMessageToClient(message.getFromUserID(), responseMessage);
        throw;
    }
}

template<typename WorkType>
void LogManager<WorkType>::storeMessage(WorkType& message) {
    try{
        // TODO: Store message in database. MessageType indicates what type of message it is.
    } catch (const std::exception& e) {
        std::cerr << "Error in storeMessage: " << e.what() << std::endl;
        throw;
    }
}

template class LogManager<Message>;
template class LogManager<MockMessage>;