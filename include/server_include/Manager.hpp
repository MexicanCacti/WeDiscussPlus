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
        virtual size_t getQueueSize() const = 0;
        virtual void setUpDatabaseConnection() = 0;
};

template<typename WorkType>
class Manager : public ManagerInterface<WorkType> {
    protected:
        std::queue<std::pair<WorkType, std::shared_ptr<tcp::socket>>> _processQueue;
        virtual void processWork(std::pair<WorkType, std::shared_ptr<tcp::socket>>& work) = 0;
    public:
        Manager(Server<WorkType>& server) : ManagerInterface<WorkType>(server) {}
        void addWork(std::pair<WorkType, std::shared_ptr<tcp::socket>>& work);
        void handleClient() override;
        inline size_t getQueueSize() const override {return _processQueue.size();}
        virtual void setUpDatabaseConnection() override = 0;
};

#endif

