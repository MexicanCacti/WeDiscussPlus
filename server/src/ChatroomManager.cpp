#include "ChatroomManager.hpp"
#include "Server.hpp"

template<typename WorkType>
std::unordered_map<int, Chatroom> ChatroomManager<WorkType>::_chatroomMap;

template<typename WorkType>
std::shared_mutex ChatroomManager<WorkType>::_chatroomMapMutex;

template<typename WorkType>
void ChatroomManager<WorkType>::setUpDatabaseConnection(){
    // Set up connection to database & set up maps!
}

template<typename WorkType>
void ChatroomManager<WorkType>::processWork(WorkType& work) {
    #ifdef _DEBUG
    std::cout << "ChatroomManager processing message type: " << Message::messageTypeToString(work.getMessageType()) << std::endl;
    #endif
            
    switch(work.getMessageType()){
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
            std::cout << "Unknown message type: " << Message::messageTypeToString(work.getMessageType()) << std::endl;
            #endif
            break;
    }
}   

template<typename WorkType>
void ChatroomManager<WorkType>::addUserToChatroom(WorkType& work){
    MessageBuilder<WorkType> responseBuilder(&work);
    try {
        #ifdef _MOCK_TESTING
        {
            responseBuilder.setMessageContents("addUserToChatroom");
            responseBuilder.setSuccessBit(true);
            responseBuilder.setFromUserID(work.getFromUserID());
            WorkType responseMessage(&responseBuilder);
            this->_server.sendMessageToClient(responseMessage.getFromUserID(), responseMessage);
            return;
        }
        #endif
        /*
        std::unique_lock<std::shared_mutex> lock(_chatroomMapMutex);
        auto chatroom = _chatroomMap.find(work.getToChatroomID());
        if(chatroom == _chatroomMap.end()){
            throw std::runtime_error("Chatroom ID: " + std::to_string(work.getToChatroomID()) + " not found");
        }
        chatroom->second.addUser(work.getToUserID());
        lock.unlock();
        */
        
        #ifdef _DEBUG
            std::cout << "ChatroomManager: Sending addUserToChatroom response to client..." << std::endl;
        #endif
        
        responseBuilder.setSuccessBit(true);
        WorkType responseMessage(&responseBuilder);
        this->_server.sendMessageToClient(work.getFromUserID(), responseMessage); // This message has the chatroomObject in it
        /*
        for(const auto& userID : chatroom->second.getUsers()){
            if(userID != work.getToUserID()){
                responseBuilder.setToUserID(userID);
                WorkType responseMessage(&responseBuilder);
                this->_server.sendMessageToClient(userID, responseMessage); // This message does not have the chatroomObject in it.
            }
        }
        */
        
        #ifdef _DEBUG
            std::cout << "ChatroomManager: Sent addUserToChatroom response to client successfully" << std::endl;
        #endif
    } catch (const std::exception& e) {
        std::cerr << "Error in addUserToChatroom: " << e.what() << std::endl;
        responseBuilder.setSuccessBit(false);
        WorkType responseMessage(&responseBuilder);
        this->_server.sendMessageToClient(work.getFromUserID(), responseMessage);
        throw;
    }
}

template<typename WorkType>
void ChatroomManager<WorkType>::sendMessageToChatroom(WorkType& work){
    MessageBuilder<WorkType> responseBuilder(&work);
    try {
        #ifdef _MOCK_TESTING
        {
            responseBuilder.setMessageContents("sendMessageToChatroom");
            responseBuilder.setSuccessBit(true);
            responseBuilder.setFromUserID(work.getFromUserID());
            WorkType responseMessage(&responseBuilder);
            this->_server.sendMessageToClient(work.getFromUserID(), responseMessage);
            return;
        }
        #endif
        
        #ifdef _DEBUG
            std::cout << "ChatroomManager: Sending sendMessageToChatroom response to client(s)..." << std::endl;
        #endif
        /*
        std::unique_lock<std::shared_mutex> lock(_chatroomMapMutex);
        auto chatroom = _chatroomMap.find(work.getToChatroomID());
        if(chatroom == _chatroomMap.end()){
            throw std::runtime_error("Chatroom ID: " + std::to_string(work.getToChatroomID()) + " not found");
        }
        if(!chatroom->second.findUser(work.getFromUserID())){
            throw std::runtime_error("User ID: " + std::to_string(work.getFromUserID()) + " not found in chatroom ID: " + std::to_string(work.getToChatroomID()));
        }
        chatroom->second.addMessage(work.getMessageContents());
        lock.unlock();
        */
        responseBuilder.setSuccessBit(true);
        responseBuilder.setFromUserID(work.getFromUserID());
        WorkType responseMessage(&responseBuilder);
        this->_server.sendMessageToClient(work.getFromUserID(), responseMessage);
        /*
        for(auto& userID : chatroom->second.getUsers()){
            responseBuilder.setToUserID(userID);
            WorkType responseMessage(&responseBuilder);
            this->_server.sendMessageToClient(userID, responseMessage);
        }
        */

        #ifdef _DEBUG
            std::cout << "ChatroomManager: Sent sendMessageToChatroom response to client successfully" << std::endl;
        #endif
    } catch (const std::exception& e) {
        std::cerr << "Error in sendMessageToChatroom: " << e.what() << std::endl;
        responseBuilder.setSuccessBit(false);
        responseBuilder.setFromUserID(work.getFromUserID());
        WorkType responseMessage(&responseBuilder);
        this->_server.sendMessageToClient(work.getFromUserID(), responseMessage);
        throw;
    }
}

template<typename WorkType>
void ChatroomManager<WorkType>::createChatroom(WorkType& work){
    MessageBuilder<WorkType> responseBuilder(&work);
    try {
        #ifdef _MOCK_TESTING
        {
            responseBuilder.setMessageContents("createChatroom");
            responseBuilder.setSuccessBit(true);
            responseBuilder.setFromUserID(work.getFromUserID()); 
            WorkType responseMessage(&responseBuilder);
            this->_server.sendMessageToClient(work.getFromUserID(), responseMessage);
            return;
        }
        #endif

        // TODO: Add chatroom to database, add to _chatroomMap
        
        responseBuilder.setSuccessBit(true);
        responseBuilder.setFromUserID(work.getFromUserID());  // Use the correct client ID
        WorkType responseMessage(&responseBuilder);
        this->_server.sendMessageToClient(work.getFromUserID(), responseMessage);
        
    } catch (const std::exception& e) {
        std::cerr << "Error in createChatroom: " << e.what() << std::endl;
        responseBuilder.setSuccessBit(false);
        responseBuilder.setFromUserID(work.getFromUserID());
        WorkType responseMessage(&responseBuilder);
        this->_server.sendMessageToClient(work.getFromUserID(), responseMessage);
        throw;
    }
}

template<typename WorkType>
void ChatroomManager<WorkType>::deleteChatroom(WorkType& work){
    MessageBuilder<WorkType> responseBuilder(&work);
    try {
        #ifdef _MOCK_TESTING
        {
            responseBuilder.setMessageContents("deleteChatroom");
            responseBuilder.setSuccessBit(true);
            responseBuilder.setFromUserID(work.getFromUserID());
            WorkType responseMessage(&responseBuilder);
            this->_server.sendMessageToClient(work.getFromUserID(), responseMessage);
            return;
        }
        #endif

        std::unique_lock<std::shared_mutex> lock(_chatroomMapMutex);
        // TODO: Delete chatroom from database, let members know that the chatroom has been deleted, remove from _chatroomMap
        lock.unlock();

        responseBuilder.setSuccessBit(true);
        WorkType responseMessage(&responseBuilder);
        this->_server.sendMessageToClient(work.getFromUserID(), responseMessage);
    } catch (const std::exception& e) {
        std::cerr << "Error in deleteChatroom: " << e.what() << std::endl;
        responseBuilder.setSuccessBit(false);
        responseBuilder.setFromUserID(work.getFromUserID());
        WorkType responseMessage(&responseBuilder);
        this->_server.sendMessageToClient(work.getFromUserID(), responseMessage);
        throw;
    }
}

template<typename WorkType>
void ChatroomManager<WorkType>::removeUserFromChatroom(WorkType& work){
    MessageBuilder<WorkType> responseBuilder(&work);
    try {
        #ifdef _MOCK_TESTING
        {
            responseBuilder.setMessageContents("removeUserFromChatroom");
            responseBuilder.setSuccessBit(true);
            responseBuilder.setFromUserID(work.getFromUserID());
            WorkType responseMessage(&responseBuilder);
            this->_server.sendMessageToClient(work.getFromUserID(), responseMessage);
            return;
        }
        #endif

        std::unique_lock<std::shared_mutex> lock(_chatroomMapMutex);
        // TODO: Remove user from chatroom, (update database?) & chatroom object, let members know that the user has been removed, remove from _chatroomMap
        lock.unlock();

        responseBuilder.setSuccessBit(true);
        WorkType responseMessage(&responseBuilder);
        this->_server.sendMessageToClient(work.getFromUserID(), responseMessage);
    } catch (const std::exception& e) {
        std::cerr << "Error in removeUserFromChatroom: " << e.what() << std::endl;
        responseBuilder.setSuccessBit(false);
        responseBuilder.setFromUserID(work.getFromUserID());
        WorkType responseMessage(&responseBuilder);
        this->_server.sendMessageToClient(work.getFromUserID(), responseMessage);
        throw;
    }
}

template class ChatroomManager<Message>;
template class ChatroomManager<MockMessage>;