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