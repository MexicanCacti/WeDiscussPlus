#ifndef DATABASE_HPP
#define DATABASE_HPP

#include <sqlite3.h>
#include <memory>
#include <string>
#include <optional>
#include <vector>
#include <variant>
#include <thread>
#include <unordered_map>
#include <mutex>

struct SQLiteDBPointerDeleter {
    void operator()(sqlite3* db) const {
        if (db) {
            sqlite3_close(db);
        }
    }
};

struct SQLiteStmtDeleter {
    void operator()(sqlite3_stmt* stmt) const {
        if (stmt) {
            sqlite3_finalize(stmt);
        }
    }
};

using SQLiteDBPointer = std::unique_ptr<sqlite3, SQLiteDBPointerDeleter>;
using SQLiteStmtPointer = std::unique_ptr<sqlite3_stmt, SQLiteStmtDeleter>;
using SQLiteValue = std::variant<std::string, int, double, std::nullptr_t>;

class Database {
    private:
        std::string _dbPath;
        std::mutex _connectionMutex;
        std::unordered_map<std::thread::id, SQLiteDBPointer> _threadConnections;
        
        // Get or create a connection for the current thread
        SQLiteDBPointer& getThreadConnection() {
            std::thread::id threadId = std::this_thread::get_id();
            
            std::lock_guard<std::mutex> lock(_connectionMutex);
            auto it = _threadConnections.find(threadId);
            if (it == _threadConnections.end()) {
                // Create new connection for this thread
                sqlite3* rawDB = nullptr;
                int rc = sqlite3_open(_dbPath.c_str(), &rawDB);
                if (rc != SQLITE_OK) {
                    throw std::runtime_error("Failed to open database connection");
                }
                _threadConnections[threadId] = SQLiteDBPointer(rawDB);
            }
            return _threadConnections[threadId];
        }

    public:
        bool getDBPointer(const std::string& dbPath) {return false;} // TODO: Fix Me
        void initDatabase(const std::string& dbPath) {
            _dbPath = dbPath;
            // Initialize the database schema using the first connection
            auto& conn = getThreadConnection();
            // ... your table creation code ...
        }

        // Prepared statement methods
        std::optional<SQLiteStmtPointer> prepareStatement(const std::string& query) {
            auto& conn = getThreadConnection();
            sqlite3_stmt* rawStmt = nullptr;
            int rc = sqlite3_prepare_v2(conn.get(), query.c_str(), -1, &rawStmt, nullptr);
            if (rc != SQLITE_OK) {
                return std::nullopt;
            }
            return SQLiteStmtPointer(rawStmt);
        }

        void bindValue(SQLiteStmtPointer& stmt, int index, const SQLiteValue& value) {
            // ... existing bindValue code ...
        }

        bool executeStatement(SQLiteStmtPointer& stmt) {
            int rc = sqlite3_step(stmt.get());
            return rc == SQLITE_DONE;
        }

        // Helper methods for common operations
        bool insertUser(const std::string& username, const std::string& password) {
            auto stmt = prepareStatement("INSERT INTO Users (username, password) VALUES (?, ?)");
            if (!stmt) return false;
            
            bindValue(*stmt, 1, username);
            bindValue(*stmt, 2, password);
            
            return executeStatement(*stmt);
        }

        // ... other methods ...
};

#endif