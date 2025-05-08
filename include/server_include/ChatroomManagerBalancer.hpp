#ifndef CHATROOMMANAGERBALANCER_HPP
#define CHATROOMMANAGERBALANCER_HPP
#include "server_include/ChatroomManager.hpp"
#include "server_include/LoadBalancer.hpp"
#include "shared_include/Message.hpp"

template<typename WorkType>
class ChatroomManagerBalancer : public LoadBalancer<WorkType>{
    private:
        virtual std::shared_ptr<Manager<WorkType>> createManager(Server& server) override;
    public:
        using LoadBalancer<WorkType>::LoadBalancer;
};

#endif