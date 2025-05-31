#pragma once

#include <unordered_map>
#include <queue>
#include <mutex>
#include <atomic>
#include <chrono>
#include <asio.hpp>
#include "Database.hpp"
#include "MessageInterface.hpp"
#include "MessageBuilder.hpp"

using asio::ip::tcp;
using namespace std::chrono_literals;

class Server;

// TODO: Fix how database pointer is handled
class ManagerInterface {
    protected:
        Server& _server;
        std::mutex _processQueueMutex;
        std::atomic<bool> _shouldDoWork = true;
        std::unique_ptr<sqlite3*> _db; 

    public:
        ManagerInterface(Server& server) : _server(server) {};
        virtual ~ManagerInterface() = default;
        virtual void handleClient() = 0;
        inline void stop() {_shouldDoWork = false;}
        virtual size_t getQueueSize() = 0;
        virtual void setUpDatabaseConnection() = 0;
        
        bool isDatabaseInitialized() const { return _db != nullptr; }
        virtual void sendRoutingTestMessage(std::shared_ptr<MessageInterface>& work, const std::string& functionName) = 0;
};

class Manager : public ManagerInterface {
    protected:
        std::queue<std::shared_ptr<MessageInterface>> _processQueue;
        virtual void processWork(std::shared_ptr<MessageInterface>& work) = 0;
    public:
        Manager(Server& server) : ManagerInterface(server) {}
        void addWork(std::shared_ptr<MessageInterface>& work);
        void handleClient() override;
        size_t getQueueSize() override;
        void setUpDatabaseConnection() override;
        void sendRoutingTestMessage(std::shared_ptr<MessageInterface>& work, const std::string& functionName) override;
        ~Manager() {_db.release();}
};
