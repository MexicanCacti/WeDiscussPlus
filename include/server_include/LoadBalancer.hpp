#ifndef LOADBALANCER_HPP
#define LOADBALANCER_HPP

#include <chrono>
using namespace std::chrono_literals;

#include "server_include/Manager.hpp"

class Server;

template<typename WorkType>
class LoadBalancer{
    protected:
        std::thread _balancerThread;
        std::unordered_map<int, std::shared_ptr<Manager<WorkType>>> _managers;
        std::unordered_map<int, std::thread> _managerThreads;
        std::queue<WorkType> _workQueue;
        std::mutex _workQueueMutex;
        bool _isRunning = true;
        inline static int _managerID= 0;

        LoadBalancer() = default;

        virtual std::shared_ptr<Manager<WorkType>> createManager(Server& server) = 0;
        void findBestManager(WorkType& work);
        void assignWork(int managerID, WorkType& work);
        void waitForWork();

    public:
        void initManagers(const int managerAmount, Server& server);
        void pushWork(WorkType& work);
        inline std::unordered_map<int, std::shared_ptr<Manager<WorkType>>> getManagers() const {return _managers;};
        void stopAllThreads();
        ~LoadBalancer();
};

#endif