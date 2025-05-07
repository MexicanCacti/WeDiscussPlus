#ifndef LOADBALANCER_HPP
#define LOADBALANCER_HPP
#include <queue>
#include <thread>
#include <unordered_map>
#include <mutex>

template<typename WorkType, typename WorkerType>
class LoadBalancer{
    protected:
        //std::weak_ptr<Server> _serverReference;
        std::queue<WorkType> _workQueue;
        std::unordered_map<int, std::shared_ptr<WorkerType>> _workers;
        std::unordered_map<int, std::thread> _workerThreads;
        std::mutex _workQueueMutex;
        bool _running = true;
        inline static int _workerID = 0;

        virtual void findBestWorker(WorkType& work);
        virtual void assignWork(int workerID, WorkType& work);
        void pushWork(WorkType& work);

    public:
        LoadBalancer(const int workerAmount);
        void waitForWork();
        void stopAllThreads();
        inline std::unordered_map<int, std::shared_ptr<WorkerType>> getWorkers() const {return _workers;}
        ~LoadBalancer();
};

#endif