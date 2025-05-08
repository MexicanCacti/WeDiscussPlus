#include "server_include/Manager.hpp"
#include "server_include/Server.hpp"

template<typename WorkType>
void Manager<WorkType>::addWork(WorkType& work){
    std::unique_lock<std::mutex> lock(_processQueueMutex);
    _processQueue.push(work);
}

template class Manager<Message>;
