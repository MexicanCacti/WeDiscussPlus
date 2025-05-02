#include "server_include/ChatroomManager.hpp"
template<typename WorkType>
void ChatroomManager<WorkType>::start(){
    // Set up connection to DB then...
    checkForWork();
}

template<typename WorkType>
void ChatroomManager<WorkType>::checkForWork(){
    try{
        while(_shouldDoWork){
            _processQueueMutex.lock();
            if(_processQueue.empty()){
                _processQueueMutex.unlock();
                std::this_thread::sleep_for(2s);
                continue;
            }
    
            WorkType workToDo = std::move(_processQueue.front());
            _processQueueMutex.unlock();
            std::unique_ptr<WorkType> work = workToDo;
            _processQueue.pop();
    
            // Go Ahead & Look at the Work Type ID to figure out what it should be doing!. Then Socket should be in WorkType to send back data to client if needed

        }      
    }
    catch(const std::exception& e){
        std::cerr << "Thread Exception: " << e.what() << std::endl;
    }
}

template<typename WorkType>
void ChatroomManager<WorkType>::addWork(WorkType& work){
    _processQueueMutex.lock();
    _processQueue.push(work);
    _processQueueMutex.unlock();
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