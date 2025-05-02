#include "server_include/LoadBalancer.hpp"
#include "server_include/Server.hpp"

template<typename WorkType, typename WorkerType>
void LoadBalancer<WorkType, WorkerType>::startWorker(){
    auto worker = std::make_shared<WorkerType>(_serverReference);
    _workers[++_workerID] = worker;
    _workerThreads[_workerID] = std::thread([worker]{
        worker->start();
    });
}

template<typename WorkType, typename WorkerType>
void LoadBalancer<WorkType, WorkerType>::findBestWorker(WorkType& work){
    int smallestQueue = INT_MAX;
    for(auto& worker : _workers){
        if(worker.second->getQueueSize() < smallestQueue) smallestQueue = worker.first;
    }
    assignWork(smallestQueue, work);
}

template<typename WorkType, typename WorkerType>
void LoadBalancer<WorkType, WorkerType>::assignWork(int workerID, WorkType& work){
    if(_workers.count(workerID)){
        _workers[workerID]->addWork(work);
    }
}

template<typename WorkType, typename WorkerType>
void LoadBalancer<WorkType, WorkerType>::pushWork(WorkType& work){
    std::unique_lock<std::mutex> queueLock(_workQueueMutex);
    _workQueue.push(work);
    queueLock.unlock();
}

template<typename WorkType, typename WorkerType>
LoadBalancer<WorkType, WorkerType>::LoadBalancer(const int workerAmount, std::weak_ptr<Server> serverReference) : _serverReference(serverReference){
    int i = workerAmount;
    // Ensure at least 1 Worker!
    do{
        startWorker();
        --i;
    }
    while(i > 0);

    waitForWork();
}

template<typename WorkType, typename WorkerType>
void LoadBalancer<WorkType, WorkerType>::waitForWork(){
    try{
        std::unique_lock<std::mutex> queueLock(_workQueueMutex);
        queueLock.unlock();
        while(_running){
            queueLock.lock();
            if(_workQueue.empty()){
                queueLock.unlock();
                std::this_thread::sleep_for(2s);
                continue;
            }
            WorkType work = _workQueue.front();
            _workQueue.pop();
            queueLock.unlock();
            findBestWorker(work);
        }
    }
    catch(const std::exception& e){
        std::cerr << "Thread Exception: " << e.what() << std::endl;
        stopAllThreads();
    }
}

template<typename WorkType, typename WorkerType>
void LoadBalancer<WorkType, WorkerType>::stopAllThreads(){
    // Stop & Join Seperate so that all threads receive stop signal quickly
    for(auto& worker : _workers){
        auto workerObject = worker.second;
        workerObject->stop();
    }

    for(auto& thread : _workerThreads){
        auto workerThread = thread.second;
        if(workerThread.joinable()) workerThread.join();
    }

    _workerThreads.clear();
    _workers.clear();
    _running = false;
}

template<typename WorkType, typename WorkerType>
LoadBalancer<WorkType, WorkerType>::~LoadBalancer(){
    stopAllThreads();
}

