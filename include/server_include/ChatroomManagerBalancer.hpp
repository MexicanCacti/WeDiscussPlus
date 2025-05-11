#ifndef CHATROOMMANAGERBALANCER_HPP
#define CHATROOMMANAGERBALANCER_HPP
#include "ChatroomManager.hpp"
#include "LoadBalancer.hpp"
#include "Message.hpp"
#include "MockMessage.hpp"

template<typename WorkType>
class ChatroomManagerBalancer : public LoadBalancer<WorkType>{
    private:
        virtual std::shared_ptr<Manager<WorkType>> createManager(Server<WorkType>& server) override;
    public:
        using LoadBalancer<WorkType>::LoadBalancer;
};

#endif