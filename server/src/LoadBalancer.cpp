#include "LoadBalancer.hpp"
#include "Server.hpp"

void LoadBalancer::initManagers(const int managerAmount, Server& server){
    for(int i = 0; i < managerAmount; ++i){
        auto manager = createManager(server);
        addManager(++_managerID, manager);
    }
}

void LoadBalancer::addManager(int id, std::shared_ptr<Manager> manager) {
    _managers[id] = manager;
    _managerThreads[id] = std::thread([this, id, manager](){
        try {
            manager->setUpDatabaseConnection();
            manager->handleClient();
        } catch (const std::exception& e) {
            std::cerr << "Manager thread exception: " << e.what() << std::endl;
        }
    });
    _managerThreads[id].detach();
}

void LoadBalancer::findBestManager(std::shared_ptr<MessageInterface>& work){
    int bestManagerID = -1;
    size_t minQueueSize = SIZE_MAX;

    for(const auto& [id, manager] : _managers){
        size_t currentQueueSize = manager->getQueueSize();
        if(currentQueueSize < minQueueSize){
            minQueueSize = currentQueueSize;
            bestManagerID = id;
        }
    }
    
    if(bestManagerID != -1){
        assignWork(bestManagerID, work);
    }
}

void LoadBalancer::assignWork(int managerID, std::shared_ptr<MessageInterface>& work){
    auto it = _managers.find(managerID);
    if(it != _managers.end()){
        it->second->addWork(work);
    }
}

void LoadBalancer::pushWork(std::shared_ptr<MessageInterface>& work){
    std::lock_guard<std::mutex> queueLock(_workQueueMutex);
    _workQueue.push(work);
}

void LoadBalancer::waitForWork(){
    if(!_isRunning) {
        _isRunning = true;
        
        while(_isRunning) {
            std::unique_lock<std::mutex> queueLock(_workQueueMutex);
            
            if(_workQueue.empty()) {
                queueLock.unlock();
                std::this_thread::sleep_for(100ms);
                continue;
            }
            
            auto work = std::move(_workQueue.front());
            _workQueue.pop();
            queueLock.unlock();
            
            try {
                findBestManager(work);
            } catch (const std::exception& e) {
                std::cerr << "Error processing work: " << e.what() << std::endl;
            }
        }
    }
}

void LoadBalancer::stopAllThreads(){
    _isRunning = false;
    
    for(auto& [id, manager] : _managers){
        if(manager) {
            manager->stop();
        }
    }

    for(auto& [id, thread] : _managerThreads){
        if(thread.joinable()) {
            thread.join();
        }
    }

    _managerThreads.clear();
    _managers.clear();
}

LoadBalancer::~LoadBalancer(){
    stopAllThreads();
}