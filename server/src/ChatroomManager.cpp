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
    #ifdef _DEBUG
        std::cout << "ChatroomManager: ADD_USER_TO_CHATROOM called" << std::endl;
    #endif
    #ifdef _MOCK_TESTING
        work.first.setMessageContents("addUserToChatroom");
        asio::write(work.second, asio::buffer(work.first.serialize()));
    #endif
}

template<typename WorkType>
void ChatroomManager<WorkType>::sendMessageToChatroom(std::pair<WorkType, std::shared_ptr<tcp::socket>>& work){
    #ifdef _DEBUG
        std::cout << "ChatroomManager: SEND_MESSAGE_TO_CHATROOM called" << std::endl;
    #endif
    #ifdef _MOCK_TESTING
        work.first.setMessageContents("sendMessageToChatroom");
        asio::write(work.second, asio::buffer(work.first.serialize()));
    #endif
}

template<typename WorkType>
void ChatroomManager<WorkType>::createChatroom(std::pair<WorkType, std::shared_ptr<tcp::socket>>& work){
    #ifdef _DEBUG
        std::cout << "ChatroomManager: CREATE_CHATROOM called" << std::endl;
    #endif
    #ifdef _MOCK_TESTING
        work.first.setMessageContents("createChatroom");
        asio::write(work.second, asio::buffer(work.first.serialize()));
    #endif
}

template<typename WorkType>
void ChatroomManager<WorkType>::deleteChatroom(std::pair<WorkType, std::shared_ptr<tcp::socket>>& work){
    #ifdef _DEBUG
        std::cout << "ChatroomManager: DELETE_CHATROOM called" << std::endl;
    #endif
    #ifdef _MOCK_TESTING
        work.first.setMessageContents("deleteChatroom");
        asio::write(work.second, asio::buffer(work.first.serialize()));
    #endif
}

template<typename WorkType>
void ChatroomManager<WorkType>::removeUserFromChatroom(std::pair<WorkType, std::shared_ptr<tcp::socket>>& work){
    #ifdef _DEBUG
        std::cout << "ChatroomManager: REMOVE_USER_FROM_CHATROOM called" << std::endl;
    #endif
    #ifdef _MOCK_TESTING
        work.first.setMessageContents("removeUserFromChatroom");
        asio::write(work.second, asio::buffer(work.first.serialize()));
    #endif
}

template class ChatroomManager<Message>;
template class ChatroomManager<MockMessage>;