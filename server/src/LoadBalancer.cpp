#include "server_include/LoadBalancer.hpp"
#include "server_include/Server.hpp"

template<typename WorkType>
void LoadBalancer<WorkType>::initManagers(const int managerAmount, Server& server){
    for(int i = 0; i < managerAmount; ++i){
        auto manager = createManager(server);
        _managers[++_managerID] = manager;
        _managerThreads[_managerID] = std::thread([this, managerID = _managerID, manager](){
            manager->setUpDatabaseConnection();
            manager->handleClient();
        });
    }
}

template<typename WorkType>
void LoadBalancer<WorkType>::findBestManager(WorkType& work){
    int bestManagerID = -1;
    int minQueueSize = INT_MAX;

    for(auto it = _managers.begin(); it != _managers.end(); ++it){
        int currentQueueSize = it->second->getQueueSize();
        if(currentQueueSize < minQueueSize){
            minQueueSize = currentQueueSize;
            bestManagerID = it->first;
        }
    }
    
    if(bestManagerID != -1){
        assignWork(bestManagerID, work);
    }
}

template<typename WorkType>
void LoadBalancer<WorkType>::assignWork(int managerID, WorkType& work){
    auto it = _managers.find(managerID);
    if(it != _managers.end()){
        it->second->addWork(work);
    }
}

template<typename WorkType>
void LoadBalancer<WorkType>::pushWork(WorkType& work){
    std::unique_lock<std::mutex> queueLock(_workQueueMutex);
    _workQueue.push(work);
    queueLock.unlock();
}

template<typename WorkType>
void LoadBalancer<WorkType>::waitForWork(){
    try{
        std::unique_lock<std::mutex> queueLock(_workQueueMutex);
        while(_isRunning){
            if(_workQueue.empty()){
                queueLock.unlock();
                std::this_thread::sleep_for(2s);
                continue;
            }
            WorkType work = _workQueue.front();
            _workQueue.pop();
            queueLock.unlock();
            findBestManager(work);
            queueLock.lock();
        }
    }
    catch(const std::exception& e){
        std::cerr << "Thread Exception: " << e.what() << std::endl;
    }
    stopAllThreads();
}

template<typename WorkType>
void LoadBalancer<WorkType>::stopAllThreads(){
    // Stop & Join Seperate so that all threads receive stop signal quickly
    for(auto& manager : _managers){
        manager.second->stop();
    }

    for(auto& [id, thread] : _managerThreads){
        if(thread.joinable()) thread.join();
    }

    _managerThreads.clear();
    _managers.clear();
    _isRunning = false;
}

template<typename WorkType>
LoadBalancer<WorkType>::~LoadBalancer(){
    stopAllThreads();
}

template class LoadBalancer<Message>;