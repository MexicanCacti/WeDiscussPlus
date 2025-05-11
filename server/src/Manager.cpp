#include "Manager.hpp"
#include "Server.hpp"

template<typename WorkType>
void Manager<WorkType>::addWork(std::pair<WorkType, std::shared_ptr<tcp::socket>>& work) {
    std::lock_guard<std::mutex> lock(this->_processQueueMutex);
    _processQueue.push(work);
}

template<typename WorkType>
void Manager<WorkType>::handleClient() {
    while(this->_shouldDoWork) {
        std::unique_lock<std::mutex> lock(this->_processQueueMutex);
        if(_processQueue.empty()) {
            lock.unlock();
            std::this_thread::sleep_for(100ms);
            continue;
        }
        
        auto work = std::move(_processQueue.front());
        _processQueue.pop();
        lock.unlock();
        
        try {
            processWork(work);
        } catch (const std::exception& e) {
            std::cerr << "Error processing work: " << e.what() << std::endl;
        }
    }
}

template class Manager<Message>;
template class Manager<MockMessage>;