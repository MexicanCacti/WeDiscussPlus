#include "server_include/ChatroomManager.hpp"
#include <chrono>
using namespace std::chrono_literals;

template<typename WorkType>
std::unordered_map<int, Chatroom> ChatroomManager<WorkType>::_chatroomMap;

template<typename WorkType>
void ChatroomManager<WorkType>::setUpDatabaseConnection(){
    // Set up connection to database & set up maps!
}

// Notes this->_processQueue is needed for proper name lookup from compiler!
template<typename WorkType>
void ChatroomManager<WorkType>::handleClient(){
    try{
        while(this->_shouldDoWork){
            this->_processQueueMutex.lock();
            if(this->_processQueue.empty()){
                this->_processQueueMutex.unlock();
                std::this_thread::sleep_for(2s);
                continue;
            }
    
            WorkType workToDo = std::move(this->_processQueue.front());
            this->_processQueueMutex.unlock();
            this->_processQueue.pop();
    
            // Go Ahead & Look at the Work Type ID to figure out what it should be doing!. Then Socket should be in WorkType to send back data to client if needed

        }      
    }
    catch(const std::exception& e){
        std::cerr << "Thread Exception: " << e.what() << std::endl;
    }
}

template<typename WorkType>
void ChatroomManager<WorkType>::addUserToChatroom(WorkType* work){
    
}

template<typename WorkType>
void ChatroomManager<WorkType>::sendMessageToChatroom(WorkType* work){
    
}

template<typename WorkType>
void ChatroomManager<WorkType>::createChatroom(WorkType* work){
    
}

template<typename WorkType>
void ChatroomManager<WorkType>::deleteChatroom(WorkType* work){
    
}

template<typename WorkType>
void ChatroomManager<WorkType>::removeUserFromChatroom(WorkType* work){
    
}

template class ChatroomManager<Message>;