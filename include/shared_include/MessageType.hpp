#pragma once

enum class MessageType{
    CONNECT, // Client->Server Initiates connection, Server->Client Connection Success
    AUTHENTICATE, // Client->Server Authenticates user, Server->Client Authentication Success
    LOGOUT, // UserManager->logoutUser
    ADD_USER, // UserManager->addUser
    CHANGE_USER_PASSWORD, // UserManager->changeUserPassword
    CHANGE_USER_NAME, // UserManager->changeUserName
    DELETE_USER, // UserManager->deleteUser
    SEND_MESSAGE_TO_USER, // UserManager->sendMessageToUser
    CREATE_CHATROOM, // ChatroomManager->createChatroom
    DELETE_CHATROOM, // ChatroomManager->deleteChatroom
    ADD_USER_TO_CHATROOM, // ChatroomManager->addUserToChatroom
    SEND_MESSAGE_TO_CHATROOM, // ChatroomManager->sendMessageToChatroom
    REMOVE_USER_FROM_CHATROOM, // ChatroomManager->removeUserFromChatroom
    GET_USER_MESSAGES, // LogManager->answerLogRequest
    GET_CHATROOM_MESSAGES, // LogManager->answerLogRequest
    STORE_MESSAGE, // LogManager->storeMessage
    UNDEFINED,
    TEST
};
