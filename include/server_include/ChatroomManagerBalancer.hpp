#pragma once
#include "ChatroomManager.hpp"
#include "LoadBalancer.hpp"
#include "MessageInterface.hpp"

template<typename WorkType>
class ChatroomManagerBalancer : public LoadBalancer<WorkType>{
    private:
        virtual std::shared_ptr<Manager<WorkType>> createManager(Server<WorkType>& server) override;
    public:
        using LoadBalancer<WorkType>::LoadBalancer;
};
