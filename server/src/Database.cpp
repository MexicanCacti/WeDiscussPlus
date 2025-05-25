#include "Database.hpp"
#include <iostream>

void Database::initDatabase(const std::string& dbPath){
    const char* UserTable = R"(
        CREATE TABLE IF NOT EXISTS Users (
            ID INTEGER PRIMARY KEY AUTOINCREMENT,
            username TEXT NOT NULL UNIQUE,
            password TEXT NOT NULL,
            inboxID INTEGER AUTOINCREMENT,
            FOREIGN KEY (inboxID) REFERENCES InboxTable(inboxKey)
        );
    )";

    const char* InboxTable = R"(
        CREATE TABLE IF NOT EXISTS InboxTable (
            inboxKey INTEGER PRIMARY KEY AUTOINCREMENT,
            userID INTEGER NOT NULL,
            messageID TEXT NOT NULL,
            FOREIGN KEY (userID) REFERENCES Users(id),
            FOREIGN KEY (messageID) REFERENCES MessageTable(messageId)
        );
    )";

    const char* ChatroomTable = R"(
        CREATE TABLE IF NOT EXISTS ChatroomTable (
            chatroomID INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL,
            created_at DATETIME DEFAULT CURRENT_TIMESTAMP
        );
    )";

    const char* ChatroomMembersTable = R"(
        CREATE TABLE IF NOT EXISTS ChatroomMembersTable (
            chatroomID INTEGER NOT NULL,
            userID INTEGER NOT NULL,
            joined_at DATETIME DEFAULT CURRENT_TIMESTAMP,
            PRIMARY KEY (chatroomID, userID),
            FOREIGN KEY (chatroomID) REFERENCES ChatroomTable(chatroomID),
            FOREIGN KEY (userID) REFERENCES Users(ID)
        );
    )";

    const char* ChatroomMessagesTable = R"(
        CREATE TABLE IF NOT EXISTS ChatroomMessagesTable (
            chatroomID INTEGER NOT NULL,
            messageID TEXT NOT NULL,
            sent_at DATETIME DEFAULT CURRENT_TIMESTAMP,
            PRIMARY KEY (chatroomID, messageID),
            FOREIGN KEY (chatroomID) REFERENCES ChatroomTable(chatroomID),
            FOREIGN KEY (messageID) REFERENCES MessageTable(messageId)
        );
    )";

    const char* MessageTable = R"(
        CREATE TABLE IF NOT EXISTS MessageTable (
            messageId TEXT PRIMARY KEY,
            messageData TEXT NOT NULL,
            senderId INTEGER NOT NULL,
            timestamp DATETIME DEFAULT CURRENT_TIMESTAMP,
            FOREIGN KEY (senderId) REFERENCES Users(id)
        );
    )";    
}

std::optional<SQLiteDBPointer> Database::getDBPointer(const std::string& dbPath){
    sqlite3* rawDB = nullptr;

    int openAttempt = sqlite3_open(dbPath.c_str(), &rawDB);
    if (openAttempt != SQLITE_OK){
        std::cerr << "Error opening database: " << sqlite3_errmsg(rawDB) << std::endl;
        return std::nullopt;
    }

    return SQLiteDBPointer(rawDB);
}

void Database::closeDatabase(SQLiteDBPointer& db){
    sqlite3_close(db.get());
}

std::optional<SQLiteStmtPointer> Database::prepareStatement(SQLiteDBPointer& db, const std::string& query) {
    sqlite3_stmt* rawStmt = nullptr;
    int rc = sqlite3_prepare_v2(db.get(), query.c_str(), -1, &rawStmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Error preparing statement: " << sqlite3_errmsg(db.get()) << std::endl;
        return std::nullopt;
    }
    return SQLiteStmtPointer(rawStmt);
}

void Database::bindValue(SQLiteStmtPointer& stmt, int index, const SQLiteValue& value) {
    switch (value.index()) {
        case 0:
            sqlite3_bind_text(stmt.get(), index, std::get<std::string>(value).c_str(), -1, SQLITE_STATIC);
            break;
        case 1:
            sqlite3_bind_int(stmt.get(), index, std::get<int>(value));
            break;  
        case 2:
            sqlite3_bind_double(stmt.get(), index, std::get<double>(value));
            break;
        case 3:
            sqlite3_bind_null(stmt.get(), index);
            break;
    }
}

bool Database::executeStatement(SQLiteStmtPointer& stmt) {
    int rc = sqlite3_step(stmt.get());
    return rc == SQLITE_DONE;
}

bool Database::insertUser(SQLiteDBPointer& db, const std::string& username, const std::string& password) {
    auto stmt = prepareStatement(db, "INSERT INTO Users (username, password) VALUES (?, ?)");
    if (!stmt) return false;
    
    bindValue(*stmt, 1, username);
    bindValue(*stmt, 2, password);
    
    return executeStatement(*stmt);
}

bool Database::insertMessage(SQLiteDBPointer& db, const std::string& messageId, const std::string& messageData, int senderId) {
    auto stmt = prepareStatement(db, "INSERT INTO MessageTable (messageId, messageData, senderId) VALUES (?, ?, ?)");
    if (!stmt) return false;
    
    bindValue(*stmt, 1, messageId);
    bindValue(*stmt, 2, messageData);
    bindValue(*stmt, 3, senderId);
    
    return executeStatement(*stmt);
}

bool Database::addMessageToInbox(SQLiteDBPointer& db, int userId, const std::string& messageId) {
    auto stmt = prepareStatement(db, "INSERT INTO InboxTable (userId, messageId) VALUES (?, ?)");
    if (!stmt) return false;
    
    bindValue(*stmt, 1, userId);
    bindValue(*stmt, 2, messageId);
    
    return executeStatement(*stmt);
}

bool Database::queryData(SQLiteDBPointer& db, const std::string& query, sqlite3_callback callback, void* data){
    return sqlite3_exec(db.get(), query.c_str(), callback, data, nullptr) == SQLITE_OK;
}

bool Database::createChatroom(SQLiteDBPointer& db, const std::string& name) {
    auto stmt = prepareStatement(db, "INSERT INTO ChatroomTable (name) VALUES (?)");
    if (!stmt) return false;
    
    bindValue(*stmt, 1, name);
    return executeStatement(*stmt);
}

bool Database::addUserToChatroom(SQLiteDBPointer& db, int chatroomId, int userId) {
    auto stmt = prepareStatement(db, "INSERT INTO ChatroomMembersTable (chatroomID, userID) VALUES (?, ?)");
    if (!stmt) return false;
    
    bindValue(*stmt, 1, chatroomId);
    bindValue(*stmt, 2, userId);
    return executeStatement(*stmt);
}

bool Database::removeUserFromChatroom(SQLiteDBPointer& db, int chatroomId, int userId) {
    auto stmt = prepareStatement(db, "DELETE FROM ChatroomMembersTable WHERE chatroomID = ? AND userID = ?");
    if (!stmt) return false;
    
    bindValue(*stmt, 1, chatroomId);
    bindValue(*stmt, 2, userId);
    return executeStatement(*stmt);
}

bool Database::addMessageToChatroom(SQLiteDBPointer& db, int chatroomId, const std::string& messageId) {
    auto stmt = prepareStatement(db, "INSERT INTO ChatroomMessagesTable (chatroomID, messageID) VALUES (?, ?)");
    if (!stmt) return false;
    
    bindValue(*stmt, 1, chatroomId);
    bindValue(*stmt, 2, messageId);
    return executeStatement(*stmt);
}

std::vector<int> Database::getChatroomMembers(SQLiteDBPointer& db, int chatroomId) {
    std::vector<int> members;
    auto stmt = prepareStatement(db, "SELECT userID FROM ChatroomMembersTable WHERE chatroomID = ?");
    if (!stmt) return members;
    
    bindValue(*stmt, 1, chatroomId);
    
    while (sqlite3_step(stmt->get()) == SQLITE_ROW) {
        members.push_back(sqlite3_column_int(stmt->get(), 0));
    }
    
    return members;
}

std::vector<std::string> Database::getChatroomMessages(SQLiteDBPointer& db, int chatroomId) {
    std::vector<std::string> messages;
    auto stmt = prepareStatement(db, 
        "SELECT m.messageId FROM MessageTable m "
        "JOIN ChatroomMessagesTable cm ON m.messageId = cm.messageID "
        "WHERE cm.chatroomID = ? "
        "ORDER BY m.timestamp ASC");
    if (!stmt) return messages;
    
    bindValue(*stmt, 1, chatroomId);
    
    while (sqlite3_step(stmt->get()) == SQLITE_ROW) {
        messages.push_back(reinterpret_cast<const char*>(sqlite3_column_text(stmt->get(), 0)));
    }
    
    return messages;
}