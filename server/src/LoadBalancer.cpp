#include "server_include/LoadBalancer.hpp"

template<typename WorkType, typename WorkerType>
void LoadBalancer<WorkType, WorkerType>::startWorker(){
    int workerID = ++_workerID;
    WorkerType worker(_serverReference);
    _workerThreads[workerID] = std::thread([worker] () mutable {
        worker.start();
    });
}

template<typename WorkType, typename WorkerType>
void LoadBalancer<WorkType, WorkerType>::findBestWorker(WorkType& work){
    int smallestQueue = INT_MAX;
    for(auto& workerThread : _workerThreads){
        if(workerThread.second.getQueueSize() < smallestQueue) smallestQueue = workerThread.first;
    }
    assignWork(smallestQueue, work);
}

template<typename WorkType, typename WorkerType>
void LoadBalancer<WorkType, WorkerType>::assignWork(int workerID, WorkType& work){
    auto workerThread = _workerThreads[workerID];
    workerThread->addWork(work);
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

template<typename WorkType, typename WorkerType>
void LoadBalancer<WorkType, WorkerType>::stopAllThreads(){
    // Stop & Join Seperate so that all threads receive stop signal quickly
    for(auto& thread : _workerThreads){
        auto workerThread = thread.second;
        workerThread->stop();
    }

    for(auto& thread : _workerThreads){
        auto workerThread = thread.second;
        if(workerThread.joinable()) workerThread.join();
    }

    _workerThreads.clear();
    _running = false;
}

template<typename WorkType, typename WorkerType>
LoadBalancer<WorkType, WorkerType>::~LoadBalancer(){
    stopAllThreads();
}

