#pragma once

#include <sqlite3.h>
#include <string>

class Database {
    private:
        std::string _dbPath;
        
        void initDatabase(const std::string& dbPath);
       

    public:
        Database(const std::string& dbPath) : _dbPath(dbPath) {initDatabase(dbPath);}

};