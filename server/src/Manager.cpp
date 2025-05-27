#include "Manager.hpp"
#include "Server.hpp"

void Manager::addWork(std::shared_ptr<MessageInterface>& work) {
    std::lock_guard<std::mutex> lock(this->_processQueueMutex);
    _processQueue.push(work);
}

void Manager::handleClient() {
    while(this->_shouldDoWork) {
        std::unique_lock<std::mutex> lock(this->_processQueueMutex);
        if(_processQueue.empty()) {
            lock.unlock();
            std::this_thread::sleep_for(100ms);
            continue;
        }
        
        auto work = _processQueue.front();
        _processQueue.pop();
        lock.unlock();
        
        try {
            processWork(work);
        } catch (const std::exception& e) {
            std::cerr << "Error processing work: " << e.what() << std::endl;
        }
    }
}

size_t Manager::getQueueSize() {
    std::lock_guard<std::mutex> lock(this->_processQueueMutex);
    return _processQueue.size();
}

void Manager::sendRoutingTestMessage(std::shared_ptr<MessageInterface>& work, const std::string& functionName) {
    MessageBuilder responseBuilder;
    responseBuilder.setMessageType(work->getMessageType());
    responseBuilder.setMessageContents(functionName);
    responseBuilder.setSuccessBit(true);
    responseBuilder.setFromUser(work->getFromUsername(), work->getFromUserID());
    responseBuilder.setToChatroom(work->getToChatroomID());
    auto responseMessage = responseBuilder.build();
    _server.sendMessageToClient(work->getToUserID(), responseMessage);
    try {
        responseBuilder.setMessageType(work->getMessageType());
        responseBuilder.setMessageContents(functionName);
        responseBuilder.setSuccessBit(true);
        responseBuilder.setFromUser(work->getFromUsername(), work->getFromUserID());
        responseBuilder.setToChatroom(work->getToChatroomID());
        auto responseMessage = responseBuilder.build();
        _server.sendMessageToClient(work->getToUserID(), responseMessage);
    } catch (const std::exception& e) {
        std::cerr << "Error in sendRoutingTestMessage: " << e.what() << std::endl;   
        responseBuilder.setSuccessBit(false);
        responseBuilder.setMessageContents("sendRoutingTestMessage");
        auto failedMessage = responseBuilder.build();
        _server.sendMessageToClient(work->getToUserID(), failedMessage);
        throw std::runtime_error("Error in sendRoutingTestMessage: " + std::string(e.what()));
    }
}