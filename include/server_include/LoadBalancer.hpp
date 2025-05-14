#ifndef LOADBALANCER_HPP
#define LOADBALANCER_HPP

#include <chrono>
#include <asio.hpp>
using asio::ip::tcp;
using namespace std::chrono_literals;

#include "Manager.hpp"

template<typename WorkType>
class Server;

template<typename WorkType>
class LoadBalancer{
    protected:
        std::thread _balancerThread;
        std::unordered_map<int, std::shared_ptr<Manager<WorkType>>> _managers;
        std::unordered_map<int, std::thread> _managerThreads;
        std::queue<WorkType> _workQueue;
        std::mutex _workQueueMutex;
        bool _isRunning = false;
        inline static int _managerID= 0;

        LoadBalancer() = default;

        virtual std::shared_ptr<Manager<WorkType>> createManager(Server<WorkType>& server) = 0;
        void findBestManager(WorkType& work);
        void assignWork(int managerID, WorkType& work);
    public:
        void waitForWork();
        void initManagers(const int managerAmount, Server<WorkType>& server);
        void addManager(int id, std::shared_ptr<Manager<WorkType>> manager);
        void pushWork(WorkType& work);
        void pushWork(WorkType&& work);
        inline std::unordered_map<int, std::shared_ptr<Manager<WorkType>>> getManagers() const {return _managers;};
        void stopAllThreads();
        ~LoadBalancer();
};

#endif