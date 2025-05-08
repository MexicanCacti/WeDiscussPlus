#ifndef MANAGER_HPP
#define MANAGER_HPP

#include <unordered_map>
#include <queue>
#include <mutex>
#include <atomic>
#include <chrono>
using namespace std::chrono_literals;

class Server;

class ManagerInterface{
    protected:
        Server& _server;
        std::mutex _processQueueMutex;
        std::atomic<bool> _shouldDoWork = true;
    public:
        ManagerInterface(Server& server) : _server(server) {};
        virtual ~ManagerInterface() = default;
        virtual void handleClient() = 0;
        inline void stop() {_shouldDoWork = false;}
        virtual size_t getQueueSize() const = 0;
        virtual void setUpDatabaseConnection() = 0;
};

template<typename WorkType>
class Manager : public ManagerInterface{
    protected:
        std::queue<WorkType> _processQueue;
    public:
        Manager(Server& server) : ManagerInterface(server) {}
        void addWork(WorkType& work);
        inline size_t getQueueSize() const override {return _processQueue.size();}
        virtual void setUpDatabaseConnection() override = 0;
};

#endif

