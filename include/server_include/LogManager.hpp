#ifndef LOGMANAGER_HPP
#define LOGMANAGER_HPP

#include "Manager.hpp"
#include <asio.hpp>
#include <memory>

using asio::ip::tcp;

template<typename WorkType>
class Server;

template<typename WorkType>
class LogManager : public Manager<WorkType> {
    private:
        void getUserMessages(int userID);
        void getChatroomMessages(int chatroomID);
        void answerLogRequest(WorkType& message);
        void processWork(WorkType& work) override;
        void setUpDatabaseConnection() override;

    public:
        void storeMessage(WorkType& message);
        LogManager(Server<WorkType>& server) : Manager<WorkType>(server) {this->setUpDatabaseConnection();}
};

#endif