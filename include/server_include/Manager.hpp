#pragma once

#include <unordered_map>
#include <queue>
#include <mutex>
#include <atomic>
#include <chrono>
#include <asio.hpp>
#include "Database.hpp"
#include "MessageInterface.hpp"

using asio::ip::tcp;
using namespace std::chrono_literals;

class Server;

class ManagerInterface {
    protected:
        Server& _server;
        std::mutex _processQueueMutex;
        std::atomic<bool> _shouldDoWork = true;
        std::shared_ptr<Database> _db; 
        
        bool initializeDatabase(const std::string& dbPath) {
            if (!_db) {
                _db = std::make_shared<Database>();
            }
            auto dbPointer = _db->getDBPointer(dbPath);
            if (!dbPointer) {
                return false;
            }
            _db->initDatabase(dbPath);
            return true;
        }

    public:
        ManagerInterface(Server& server) : _server(server) {};
        virtual ~ManagerInterface() = default;
        virtual void handleClient() = 0;
        inline void stop() {_shouldDoWork = false;}
        virtual size_t getQueueSize() = 0;
        virtual void setUpDatabaseConnection() = 0;
        
        Database& getDatabase() { return *_db; }
        bool isDatabaseInitialized() const { return _db != nullptr; }
        
        void setDatabase(std::shared_ptr<Database> db) {
            _db = db;
        }
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
        virtual void setUpDatabaseConnection() override = 0;
};
