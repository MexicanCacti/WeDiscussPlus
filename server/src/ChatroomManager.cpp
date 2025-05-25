#include "ChatroomManager.hpp"
#include "Server.hpp"
#include "MessageBuilder.hpp"

std::unordered_map<int, Chatroom> ChatroomManager::_chatroomMap;
std::shared_mutex ChatroomManager::_chatroomMapMutex;

void ChatroomManager::setUpDatabaseConnection() {
    // Set up connection to database & set up maps!
}

void ChatroomManager::processWork(std::shared_ptr<MessageInterface>& work) {
    #ifdef _DEBUG
    std::cout << "ChatroomManager processing message type: " << MessageInterface::messageTypeToString(work->getMessageType()) << std::endl;
    #endif
            
    switch(work->getMessageType()) {
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
            std::cout << "Unknown message type: " << MessageInterface::messageTypeToString(work->getMessageType()) << std::endl;
            #endif
            break;
    }
}

void ChatroomManager::addUserToChatroom(std::shared_ptr<MessageInterface>& work) {
    MessageBuilder responseBuilder;
    try {
        #ifdef _MOCK_TESTING
        {
            responseBuilder.setMessageContents("addUserToChatroom")
                         .setSuccessBit(true)
                         .setFromUser(work->getFromUsername(), work->getFromUserID());
            auto responseMessage = responseBuilder.buildChatroomMessage();
            this->_server.sendMessageToClient(responseMessage->getFromUserID(), responseMessage);
            return;
        }
        #endif
        
        #ifdef _DEBUG
            std::cout << "ChatroomManager: Sending addUserToChatroom response to client..." << std::endl;
        #endif
        
        responseBuilder.setSuccessBit(true)
                      .setFromUser(work->getFromUsername(), work->getFromUserID());
        auto responseMessage = responseBuilder.buildChatroomMessage();
        this->_server.sendMessageToClient(work->getFromUserID(), responseMessage);
        
        #ifdef _DEBUG
            std::cout << "ChatroomManager: Sent addUserToChatroom response to client successfully" << std::endl;
        #endif
    } catch (const std::exception& e) {
        std::cerr << "Error in addUserToChatroom: " << e.what() << std::endl;
        responseBuilder.setSuccessBit(false)
                      .setFromUser(work->getFromUsername(), work->getFromUserID());
        auto responseMessage = responseBuilder.buildChatroomMessage();
        this->_server.sendMessageToClient(work->getFromUserID(), responseMessage);
        throw;
    }
}

void ChatroomManager::sendMessageToChatroom(std::shared_ptr<MessageInterface>& work) {
    MessageBuilder responseBuilder;
    try {
        #ifdef _MOCK_TESTING
        {
            responseBuilder.setMessageContents("sendMessageToChatroom")
                         .setSuccessBit(true)
                         .setFromUser(work->getFromUsername(), work->getFromUserID());
            auto responseMessage = responseBuilder.buildChatroomMessage();
            this->_server.sendMessageToClient(work->getFromUserID(), responseMessage);
            return;
        }
        #endif
        
        #ifdef _DEBUG
            std::cout << "ChatroomManager: Sending sendMessageToChatroom response to client(s)..." << std::endl;
        #endif
        
        responseBuilder.setSuccessBit(true)
                      .setFromUser(work->getFromUsername(), work->getFromUserID());
        auto responseMessage = responseBuilder.buildChatroomMessage();
        this->_server.sendMessageToClient(work->getFromUserID(), responseMessage);
        
    } catch (const std::exception& e) {
        std::cerr << "Error in sendMessageToChatroom: " << e.what() << std::endl;
        responseBuilder.setSuccessBit(false)
                      .setFromUser(work->getFromUsername(), work->getFromUserID());
        auto responseMessage = responseBuilder.buildChatroomMessage();
        this->_server.sendMessageToClient(work->getFromUserID(), responseMessage);
        throw;
    }
}

void ChatroomManager::createChatroom(std::shared_ptr<MessageInterface>& work) {
    MessageBuilder responseBuilder;
    try {
        #ifdef _MOCK_TESTING
        {
            responseBuilder.setMessageContents("createChatroom")
                         .setSuccessBit(true)
                         .setFromUser(work->getFromUsername(), work->getFromUserID());
            auto responseMessage = responseBuilder.buildChatroomMessage();
            this->_server.sendMessageToClient(work->getFromUserID(), responseMessage);
            return;
        }
        #endif

        // TODO: Add chatroom to database, add to _chatroomMap
        
        responseBuilder.setSuccessBit(true)
                      .setFromUser(work->getFromUsername(), work->getFromUserID());
        auto responseMessage = responseBuilder.buildChatroomMessage();
        this->_server.sendMessageToClient(work->getFromUserID(), responseMessage);
        
    } catch (const std::exception& e) {
        std::cerr << "Error in createChatroom: " << e.what() << std::endl;
        responseBuilder.setSuccessBit(false)
                      .setFromUser(work->getFromUsername(), work->getFromUserID());
        auto responseMessage = responseBuilder.buildChatroomMessage();
        this->_server.sendMessageToClient(work->getFromUserID(), responseMessage);
        throw;
    }
}

void ChatroomManager::deleteChatroom(std::shared_ptr<MessageInterface>& work) {
    MessageBuilder responseBuilder;
    try {
        #ifdef _MOCK_TESTING
        {
            responseBuilder.setMessageContents("deleteChatroom")
                         .setSuccessBit(true)
                         .setFromUser(work->getFromUsername(), work->getFromUserID());
            auto responseMessage = responseBuilder.buildChatroomMessage();
            this->_server.sendMessageToClient(work->getFromUserID(), responseMessage);
            return;
        }
        #endif

        std::unique_lock<std::shared_mutex> lock(_chatroomMapMutex);
        // TODO: Delete chatroom from database, let members know that the chatroom has been deleted, remove from _chatroomMap
        lock.unlock();

        responseBuilder.setSuccessBit(true)
                      .setFromUser(work->getFromUsername(), work->getFromUserID());
        auto responseMessage = responseBuilder.buildChatroomMessage();
        this->_server.sendMessageToClient(work->getFromUserID(), responseMessage);
    } catch (const std::exception& e) {
        std::cerr << "Error in deleteChatroom: " << e.what() << std::endl;
        responseBuilder.setSuccessBit(false)
                      .setFromUser(work->getFromUsername(), work->getFromUserID());
        auto responseMessage = responseBuilder.buildChatroomMessage();
        this->_server.sendMessageToClient(work->getFromUserID(), responseMessage);
        throw;
    }
}

void ChatroomManager::removeUserFromChatroom(std::shared_ptr<MessageInterface>& work) {
    MessageBuilder responseBuilder;
    try {
        #ifdef _MOCK_TESTING
        {
            responseBuilder.setMessageContents("removeUserFromChatroom")
                         .setSuccessBit(true)
                         .setFromUser(work->getFromUsername(), work->getFromUserID());
            auto responseMessage = responseBuilder.buildChatroomMessage();
            this->_server.sendMessageToClient(work->getFromUserID(), responseMessage);
            return;
        }
        #endif

        std::unique_lock<std::shared_mutex> lock(_chatroomMapMutex);
        // TODO: Remove user from chatroom, (update database?) & chatroom object, let members know that the user has been removed, remove from _chatroomMap
        lock.unlock();

        responseBuilder.setSuccessBit(true)
                      .setFromUser(work->getFromUsername(), work->getFromUserID());
        auto responseMessage = responseBuilder.buildChatroomMessage();
        this->_server.sendMessageToClient(work->getFromUserID(), responseMessage);
    } catch (const std::exception& e) {
        std::cerr << "Error in removeUserFromChatroom: " << e.what() << std::endl;
        responseBuilder.setSuccessBit(false)
                      .setFromUser(work->getFromUsername(), work->getFromUserID());
        auto responseMessage = responseBuilder.buildChatroomMessage();
        this->_server.sendMessageToClient(work->getFromUserID(), responseMessage);
        throw;
    }
}