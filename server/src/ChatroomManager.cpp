#include "ChatroomManager.hpp"
#include <chrono>
using namespace std::chrono_literals;

template<typename WorkType>
std::unordered_map<int, Chatroom> ChatroomManager<WorkType>::_chatroomMap;

template<typename WorkType>
void ChatroomManager<WorkType>::setUpDatabaseConnection(){
    // Set up connection to database & set up maps!
}

template<typename WorkType>
void ChatroomManager<WorkType>::processWork(std::pair<WorkType, std::shared_ptr<tcp::socket>>& work) {
    #ifdef _DEBUG
    std::cout << "ChatroomManager processing message type: " << Message::messageTypeToString(work.first.getMessageType()) << std::endl;
    #endif
            
    switch(work.first.getMessageType()){
        case MessageType::CREATE_CHATROOM:
            createChatroom(work);
            break;
        case MessageType::DELETE_CHATROOM:
            deleteChatroom(work);
            break;
        case MessageType::ADD_USER_TO_CHATROOM:
            addUserToChatroom(work);
            break;
        case MessageType::SEND_MESSAGE_TO_CHATROOM:
            sendMessageToChatroom(work);
            break;
        case MessageType::REMOVE_USER_FROM_CHATROOM:
            removeUserFromChatroom(work);
            break;
        default:
            #ifdef _DEBUG
            std::cout << "Unknown message type: " << Message::messageTypeToString(work.first.getMessageType()) << std::endl;
            #endif
            break;
    }
}   

template<typename WorkType>
void ChatroomManager<WorkType>::addUserToChatroom(std::pair<WorkType, std::shared_ptr<tcp::socket>>& work){
    std::cout << "ChatroomManager: ADD_USER_TO_CHATROOM called" << std::endl;
    try {
        MessageBuilder<WorkType> responseBuilder(&work.first);
        #ifndef _MOCK_TESTING
        responseBuilder.setMessageContents("addUserToChatroom");
        #endif
        WorkType responseMessage(&responseBuilder);
        
        std::cout << "ChatroomManager: Serializing addUserToChatroom response..." << std::endl;
        std::vector<char> responseData = responseMessage.serialize();
        std::cout << "ChatroomManager: Sending addUserToChatroom response..." << std::endl;
        asio::write(*work.second, asio::buffer(responseData));
        std::cout << "ChatroomManager: Sent addUserToChatroom response successfully" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error in addUserToChatroom: " << e.what() << std::endl;
        throw;
    }
}

template<typename WorkType>
void ChatroomManager<WorkType>::sendMessageToChatroom(std::pair<WorkType, std::shared_ptr<tcp::socket>>& work){
    std::cout << "ChatroomManager: SEND_MESSAGE_TO_CHATROOM called" << std::endl;
    try {
        MessageBuilder<WorkType> responseBuilder(&work.first);
        #ifndef _MOCK_TESTING
        responseBuilder.setMessageContents("sendMessageToChatroom");
        #endif
        WorkType responseMessage(&responseBuilder);
        
        std::cout << "ChatroomManager: Serializing sendMessageToChatroom response..." << std::endl;
        std::vector<char> responseData = responseMessage.serialize();
        std::cout << "ChatroomManager: Sending sendMessageToChatroom response..." << std::endl;
        asio::write(*work.second, asio::buffer(responseData));
        std::cout << "ChatroomManager: Sent sendMessageToChatroom response successfully" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error in sendMessageToChatroom: " << e.what() << std::endl;
        throw;
    }
}

template<typename WorkType>
void ChatroomManager<WorkType>::createChatroom(std::pair<WorkType, std::shared_ptr<tcp::socket>>& work){
    std::cout << "ChatroomManager: CREATE_CHATROOM called" << std::endl;
    try {
        MessageBuilder<WorkType> responseBuilder(&work.first);
        #ifndef _MOCK_TESTING
        responseBuilder.setMessageContents("createChatroom");
        #endif
        WorkType responseMessage(&responseBuilder);
        
        std::cout << "ChatroomManager: Serializing createChatroom response..." << std::endl;
        std::vector<char> responseData = responseMessage.serialize();
        std::cout << "ChatroomManager: Sending createChatroom response..." << std::endl;
        asio::write(*work.second, asio::buffer(responseData));
        std::cout << "ChatroomManager: Sent createChatroom response successfully" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error in createChatroom: " << e.what() << std::endl;
        throw;
    }
}

template<typename WorkType>
void ChatroomManager<WorkType>::deleteChatroom(std::pair<WorkType, std::shared_ptr<tcp::socket>>& work){
    std::cout << "ChatroomManager: DELETE_CHATROOM called" << std::endl;
    try {
        MessageBuilder<WorkType> responseBuilder(&work.first);
        #ifndef _MOCK_TESTING
        responseBuilder.setMessageContents("deleteChatroom");
        #endif
        WorkType responseMessage(&responseBuilder);
        
        std::cout << "ChatroomManager: Serializing deleteChatroom response..." << std::endl;
        std::vector<char> responseData = responseMessage.serialize();
        std::cout << "ChatroomManager: Sending deleteChatroom response..." << std::endl;
        asio::write(*work.second, asio::buffer(responseData));
        std::cout << "ChatroomManager: Sent deleteChatroom response successfully" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error in deleteChatroom: " << e.what() << std::endl;
        throw;
    }
}

template<typename WorkType>
void ChatroomManager<WorkType>::removeUserFromChatroom(std::pair<WorkType, std::shared_ptr<tcp::socket>>& work){
    std::cout << "ChatroomManager: REMOVE_USER_FROM_CHATROOM called" << std::endl;
    try {
        MessageBuilder<WorkType> responseBuilder(&work.first);
        #ifndef _MOCK_TESTING
        responseBuilder.setMessageContents("removeUserFromChatroom");
        #endif
        WorkType responseMessage(&responseBuilder);
        
        std::cout << "ChatroomManager: Serializing removeUserFromChatroom response..." << std::endl;
        std::vector<char> responseData = responseMessage.serialize();
        std::cout << "ChatroomManager: Sending removeUserFromChatroom response..." << std::endl;
        asio::write(*work.second, asio::buffer(responseData));
        std::cout << "ChatroomManager: Sent removeUserFromChatroom response successfully" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error in removeUserFromChatroom: " << e.what() << std::endl;
        throw;
    }
}

template class ChatroomManager<Message>;
template class ChatroomManager<MockMessage>;