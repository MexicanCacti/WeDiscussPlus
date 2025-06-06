CREATE TABLE IF NOT EXISTS Users (
    ID INTEGER PRIMARY KEY AUTOINCREMENT,
    username TEXT NOT NULL UNIQUE,
    password TEXT NOT NULL,
    lastOnline TEXT DEFAULT CURRENT_TIMESTAMP,
    onlineStatus BOOLEAN DEFAULT FALSE
);

CREATE TABLE IF NOT EXISTS UserInbox (
    messageID INTEGER PRIMARY KEY AUTOINCREMENT,
    fromUserID INTEGER NOT NULL,
    toUserID INTEGER NOT NULL,
    messageContent TEXT NOT NULL,
    timestamp TEXT DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (fromUserID) REFERENCES Users(ID),
    FOREIGN KEY (toUserID) REFERENCES Users(ID)
);

CREATE TABLE IF NOT EXISTS Chatrooms (
    chatroomID INTEGER PRIMARY KEY AUTOINCREMENT,
    createdBy INTEGER NOT NULL,
    createdAt TEXT DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (createdBy) REFERENCES Users(ID)
);

CREATE TABLE IF NOT EXISTS ChatroomMembers (
    chatroomID INTEGER NOT NULL,
    userID INTEGER NOT NULL,
    joinedAt TEXT DEFAULT CURRENT_TIMESTAMP,
    PRIMARY KEY (chatroomID, userID),
    FOREIGN KEY (chatroomID) REFERENCES Chatrooms(chatroomID),
    FOREIGN KEY (userID) REFERENCES Users(ID)
);

CREATE TABLE IF NOT EXISTS ChatroomMessages (
    messageID INTEGER PRIMARY KEY AUTOINCREMENT,
    chatroomID INTEGER NOT NULL,
    fromUserID INTEGER NOT NULL,
    messageContent TEXT NOT NULL,
    timestamp TEXT DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (chatroomID) REFERENCES Chatrooms(chatroomID),
    FOREIGN KEY (fromUserID) REFERENCES Users(ID)
);

CREATE TABLE IF NOT EXISTS ConnectionLogs (
    logID INTEGER PRIMARY KEY AUTOINCREMENT,
    ipAddress TEXT NOT NULL,
    attemptedAt TEXT DEFAULT CURRENT_TIMESTAMP,
    timesAttempted INTEGER DEFAULT 0,
    lastAttemptedAt TEXT DEFAULT CURRENT_TIMESTAMP
);
