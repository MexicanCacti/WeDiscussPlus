#ifndef LOGMANAGER_HPP
#define LOGMANAGER_HPP
#include "server_include/Manager.hpp"
#include "shared_include/Message.hpp"

class Server;

template<typename WorkType>
class LogManager : public Manager<WorkType> {
    private:
        void getUserMessages(int userID);
        void getChatroomMessages(int chatroomID);
        void answerLogRequest(WorkType& message);
        void storeMessage(WorkType& message);
        void setUpDatabaseConnection() override;
    public:
        LogManager(Server& server) : Manager<WorkType>(server) {
            this->setUpDatabaseConnection();
        }
        void handleClient() override;
};

#endif