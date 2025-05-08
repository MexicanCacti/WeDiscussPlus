#include "server_include/LogManager.hpp"
#include "server_include/Server.hpp"
#include <chrono>
using namespace std::chrono_literals;

template<typename WorkType>
void LogManager<WorkType>::setUpDatabaseConnection(){
    // Set up connection to database & set up maps!
}

// Notes this->_processQueue is needed for proper name lookup from compiler!
template<typename WorkType>
void LogManager<WorkType>::handleClient(){
    try{
        while(this->_shouldDoWork){
            this->_processQueueMutex.lock();
            if(this->_processQueue.empty()){
                this->_processQueueMutex.unlock();
                std::this_thread::sleep_for(2s);
                continue;
            }

            WorkType workToDo = std::move(this->_processQueue.front());
            this->_processQueue.pop();        
            this->_processQueueMutex.unlock();

            answerLogRequest(workToDo);
        }
    }
    catch(const std::exception& e){
        std::cerr << "Thread Exception: " << e.what() << std::endl;
    }
}

template<typename WorkType>
void LogManager<WorkType>::getUserMessages(int userID) {
    // Implementation
}

template<typename WorkType>
void LogManager<WorkType>::getChatroomMessages(int chatroomID) {
    // Implementation
}

template<typename WorkType>
void LogManager<WorkType>::answerLogRequest(WorkType& message) {
    // Implementation
}

template<typename WorkType>
void LogManager<WorkType>::storeMessage(WorkType& message) {
    // Implementation
}

template class LogManager<Message>;