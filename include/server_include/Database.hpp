#pragma once

#include <iostream>
#include <fstream>
#include <sqlite3.h>
#include <string>
#include <filesystem>

class Database {
    private:
        std::string _dbPath;
        sqlite3* _db;

        bool initDatabase(const std::string& dbPath, const std::string& schemaPath);

    public:
        Database(const std::string& dbPath, const std::string& schemaPath = "schema.sql") : _dbPath(dbPath) {initDatabase(dbPath, schemaPath);}
        ~Database() {if (isConnected()) close();}
        sqlite3* getDatabaseConnection() const;
        bool isConnected() const;
        void close();

        // UserManager Functions

        // ChatroomManager Functions

        // LogManager Functions


};