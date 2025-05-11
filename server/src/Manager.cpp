#include "server_include/Manager.hpp"
#include "server_include/Server.hpp"

template<typename WorkType>
void Manager<WorkType>::addWork(std::pair<WorkType, std::shared_ptr<tcp::socket>>& work){
    std::unique_lock<std::mutex> lock(_processQueueMutex);
    _processQueue.push(work);
}

template<typename WorkType>
void Manager<WorkType>::handleClient() {
    try {
        while(this->_shouldDoWork) {
            this->_processQueueMutex.lock();
            if(this->_processQueue.empty()) {
                this->_processQueueMutex.unlock();
                std::this_thread::sleep_for(2s);
                continue;
            }

            #ifdef _DEBUG
            std::cout << "Processing work from queue. Queue size: " << _processQueue.size() << std::endl;
            #endif

            auto workToDo = std::move(this->_processQueue.front());
            this->_processQueue.pop();
            this->_processQueueMutex.unlock();

            processWork(workToDo);
        }
    }
    catch(const std::exception& e) {
        std::cerr << "Thread Exception: " << e.what() << std::endl;
    }
}

template class Manager<Message>;