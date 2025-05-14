#ifndef MANAGER_HPP
#define MANAGER_HPP

#include <unordered_map>
#include <queue>
#include <mutex>
#include <atomic>
#include <chrono>
#include <asio.hpp>
using asio::ip::tcp;
using namespace std::chrono_literals;

template<typename WorkType>
class Server;

template<typename WorkType>
class ManagerInterface {
    protected:
        Server<WorkType>& _server;
        std::mutex _processQueueMutex;
        std::atomic<bool> _shouldDoWork = true;
    public:
        ManagerInterface(Server<WorkType>& server) : _server(server) {};
        virtual ~ManagerInterface() = default;
        virtual void handleClient() = 0;
        inline void stop() {_shouldDoWork = false;}
        virtual size_t getQueueSize() = 0;
        virtual void setUpDatabaseConnection() = 0;
};

template<typename WorkType>
class Manager : public ManagerInterface<WorkType> {
    protected:
        std::queue<WorkType> _processQueue;
        virtual void processWork(WorkType& work) = 0;
    public:
        Manager(Server<WorkType>& server) : ManagerInterface<WorkType>(server) {}
        void addWork(WorkType& work);
        void handleClient() override;
        size_t getQueueSize() override;
        virtual void setUpDatabaseConnection() override = 0;
};

#endif

