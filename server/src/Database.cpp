#include "Database.hpp"
#include <filesystem>

bool Database::initDatabase(const std::string& dbPath, const std::string& schemaPath) {
    #ifdef _DEBUG
    std::cout << "Initializing database with path: " << dbPath << std::endl;
    std::cout << "Schema path: " << schemaPath << std::endl;
    #endif

    std::filesystem::path absDbPath = std::filesystem::absolute(dbPath);
    std::filesystem::path absSchemaPath = std::filesystem::absolute(schemaPath);
    
    #ifdef _DEBUG
    std::cout << "Absolute database path: " << absDbPath << std::endl;
    std::cout << "Absolute schema path: " << absSchemaPath << std::endl;
    #endif

    std::filesystem::create_directories(absDbPath.parent_path());
    #ifdef _DEBUG
    std::cout << "Database directory created/verified at: " << absDbPath.parent_path() << std::endl;
    #endif

    if (sqlite3_open(absDbPath.string().c_str(), &_db) != SQLITE_OK) {
        std::cerr << "Can't open DB: " << sqlite3_errmsg(_db) << std::endl;
        return false;
    }
    #ifdef _DEBUG
    std::cout << "Database opened successfully" << std::endl;
    #endif

    std::ifstream schemaFile(absSchemaPath);
    if (!schemaFile.is_open()) {
        std::cerr << "Could not open schema file: " << absSchemaPath << std::endl;
        return false;
    }
    #ifdef _DEBUG
    std::cout << "Schema file opened successfully" << std::endl;
    #endif

    std::string sql((std::istreambuf_iterator<char>(schemaFile)),
                    std::istreambuf_iterator<char>());
    #ifdef _DEBUG
    std::cout << "Schema SQL loaded: " << sql.substr(0, 100) << "..." << std::endl;
    #endif

    char* errMsg = nullptr;
    int rc = sqlite3_exec(_db, sql.c_str(), nullptr, nullptr, &errMsg);

    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return false;
    }
    #ifdef _DEBUG
    std::cout << "Schema executed successfully" << std::endl;
    #endif

    return true;
}

bool Database::isConnected() const {
    return _db != nullptr;
}

sqlite3* Database::getDatabaseConnection() const {
    sqlite3* db = nullptr;
    std::filesystem::path absDbPath = std::filesystem::absolute(_dbPath);
    if(sqlite3_open(absDbPath.string().c_str(), &db) != SQLITE_OK){
        std::cerr << "Can't open DB: " << sqlite3_errmsg(db) << std::endl;
        return nullptr;
    }
    return db;
}

void Database::close() {
    if(_db) {
        sqlite3_close(_db);
        _db = nullptr;
    }
}