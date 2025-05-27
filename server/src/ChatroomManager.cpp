#include "ChatroomManager.hpp"
#include "Server.hpp"

std::unordered_map<int, Chatroom> ChatroomManager::_chatroomMap;
std::shared_mutex ChatroomManager::_chatroomMapMutex;

void ChatroomManager::setUpDatabaseConnection() {
    // Set up connection to database & set up maps!
}

void ChatroomManager::processWork(std::shared_ptr<MessageInterface>& work) {
    #ifdef _DEBUG
    std::cout << "ChatroomManager processing message type: " << MessageInterface::messageTypeToString(work->getMessageType()) << std::endl;
    #endif  
    try {
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
                break;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error in processWork: " << e.what() << std::endl;
    }
}

void ChatroomManager::addUserToChatroom(std::shared_ptr<MessageInterface>& work) {
    #ifdef _MOCK_TESTING
        sendRoutingTestMessage(work, "addUserToChatroom");
        return;
    #endif

    MessageBuilder responseBuilder;
    responseBuilder.setMessageType(MessageType::ADD_USER_TO_CHATROOM);
    try {
        responseBuilder.setMessageContents("addUserToChatroom");
        responseBuilder.setSuccessBit(true);
        responseBuilder.setFromUser(work->getFromUsername(), work->getFromUserID());
        auto responseMessage = responseBuilder.build();
        this->_server.sendMessageToClient(work->getFromUserID(), responseMessage);
        
    } catch (const std::exception& e) {
        std::cerr << "Error in addUserToChatroom: " << e.what() << std::endl;
        responseBuilder.setSuccessBit(false);
        responseBuilder.setFromUser(work->getFromUsername(), work->getFromUserID());
        auto responseMessage = responseBuilder.build();
        this->_server.sendMessageToClient(work->getFromUserID(), responseMessage);
        throw std::runtime_error("Error in addUserToChatroom: " + std::string(e.what()));
    }
}

void ChatroomManager::sendMessageToChatroom(std::shared_ptr<MessageInterface>& work) {
    #ifdef _MOCK_TESTING
        sendRoutingTestMessage(work, "sendMessageToChatroom");
        return;
    #endif

    MessageBuilder responseBuilder;
    responseBuilder.setMessageType(MessageType::SEND_MESSAGE_TO_CHATROOM);
    try {     
        responseBuilder.setMessageContents("sendMessageToChatroom");
        responseBuilder.setSuccessBit(true);
        responseBuilder.setFromUser(work->getFromUsername(), work->getFromUserID());
        auto responseMessage = responseBuilder.build();
        this->_server.sendMessageToClient(work->getFromUserID(), responseMessage);
        
    } catch (const std::exception& e) {
        std::cerr << "Error in sendMessageToChatroom: " << e.what() << std::endl;
        responseBuilder.setSuccessBit(false);
        responseBuilder.setFromUser(work->getFromUsername(), work->getFromUserID());
        auto responseMessage = responseBuilder.build();
        this->_server.sendMessageToClient(work->getFromUserID(), responseMessage);
        throw std::runtime_error("Error in sendMessageToChatroom: " + std::string(e.what()));
    }
}

void ChatroomManager::createChatroom(std::shared_ptr<MessageInterface>& work) {
    #ifdef _MOCK_TESTING
        sendRoutingTestMessage(work, "createChatroom");
        return;
    #endif

    MessageBuilder responseBuilder;
    responseBuilder.setMessageType(MessageType::CREATE_CHATROOM);
    try {
        // TODO: Add chatroom to database, add to _chatroomMap
        responseBuilder.setMessageContents("createChatroom");
        responseBuilder.setSuccessBit(true);
        responseBuilder.setFromUser(work->getFromUsername(), work->getFromUserID());
        auto responseMessage = responseBuilder.build();
        this->_server.sendMessageToClient(work->getFromUserID(), responseMessage);
        
    } catch (const std::exception& e) {
        std::cerr << "Error in createChatroom: " << e.what() << std::endl;
        responseBuilder.setSuccessBit(false);
        responseBuilder.setFromUser(work->getFromUsername(), work->getFromUserID());
        auto responseMessage = responseBuilder.build();
        this->_server.sendMessageToClient(work->getFromUserID(), responseMessage);
        throw std::runtime_error("Error in createChatroom: " + std::string(e.what()));
    }
}

void ChatroomManager::deleteChatroom(std::shared_ptr<MessageInterface>& work) {
    #ifdef _MOCK_TESTING
        sendRoutingTestMessage(work, "deleteChatroom");
        return;
    #endif

    MessageBuilder responseBuilder;
    responseBuilder.setMessageType(MessageType::DELETE_CHATROOM);
    try {
        std::unique_lock<std::shared_mutex> lock(_chatroomMapMutex);
        // TODO: Delete chatroom from database, let members know that the chatroom has been deleted, remove from _chatroomMap
        lock.unlock();

        responseBuilder.setMessageContents("deleteChatroom");
        responseBuilder.setSuccessBit(true);
        responseBuilder.setFromUser(work->getFromUsername(), work->getFromUserID());
        auto responseMessage = responseBuilder.build();
        this->_server.sendMessageToClient(work->getFromUserID(), responseMessage);
    } catch (const std::exception& e) {
        std::cerr << "Error in deleteChatroom: " << e.what() << std::endl;
        responseBuilder.setSuccessBit(false);
        responseBuilder.setFromUser(work->getFromUsername(), work->getFromUserID());
        auto responseMessage = responseBuilder.build();
        this->_server.sendMessageToClient(work->getFromUserID(), responseMessage);
        throw std::runtime_error("Error in deleteChatroom: " + std::string(e.what()));
    }
}

void ChatroomManager::removeUserFromChatroom(std::shared_ptr<MessageInterface>& work) {
    #ifdef _MOCK_TESTING
        sendRoutingTestMessage(work, "removeUserFromChatroom");
        return;
    #endif

    MessageBuilder responseBuilder;
    responseBuilder.setMessageType(MessageType::REMOVE_USER_FROM_CHATROOM);
    try {
        std::unique_lock<std::shared_mutex> lock(_chatroomMapMutex);
        // TODO: Remove user from chatroom, (update database?) & chatroom object, let members know that the user has been removed, remove from _chatroomMap
        lock.unlock();

        responseBuilder.setMessageContents("removeUserFromChatroom");
        responseBuilder.setSuccessBit(true);
        responseBuilder.setFromUser(work->getFromUsername(), work->getFromUserID());
        auto responseMessage = responseBuilder.build();
        this->_server.sendMessageToClient(work->getFromUserID(), responseMessage);
    } catch (const std::exception& e) {
        std::cerr << "Error in removeUserFromChatroom: " << e.what() << std::endl;
        responseBuilder.setSuccessBit(false);
        responseBuilder.setFromUser(work->getFromUsername(), work->getFromUserID());
        auto responseMessage = responseBuilder.build();
        this->_server.sendMessageToClient(work->getFromUserID(), responseMessage);
        throw std::runtime_error("Error in removeUserFromChatroom: " + std::string(e.what()));
    }
}