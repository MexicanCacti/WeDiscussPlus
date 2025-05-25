#pragma once

#include <chrono>
#include <asio.hpp>
using asio::ip::tcp;
using namespace std::chrono_literals;

#include "Manager.hpp"
#include "MessageInterface.hpp"

class Server;

class LoadBalancer{
    protected:
        std::thread _balancerThread;
        std::unordered_map<int, std::shared_ptr<Manager>> _managers;
        std::unordered_map<int, std::thread> _managerThreads;
        std::queue<std::shared_ptr<MessageInterface>> _workQueue;
        std::mutex _workQueueMutex;
        bool _isRunning = false;
        inline static int _managerID= 0;

        LoadBalancer() = default;

        virtual std::shared_ptr<Manager> createManager(Server& server) = 0;
        void findBestManager(std::shared_ptr<MessageInterface>& work);
        void assignWork(int managerID, std::shared_ptr<MessageInterface>& work);
    public:
        void waitForWork();
        void initManagers(const int managerAmount, Server& server);
        void addManager(int id, std::shared_ptr<Manager> manager);
        void pushWork(std::shared_ptr<MessageInterface>& work);
        void pushWork(std::shared_ptr<MessageInterface>&& work);
        inline std::unordered_map<int, std::shared_ptr<Manager>> getManagers() const {return _managers;};
        void stopAllThreads();
        ~LoadBalancer();
};
