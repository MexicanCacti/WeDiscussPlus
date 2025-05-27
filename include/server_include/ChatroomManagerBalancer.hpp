#pragma once
#include "ChatroomManager.hpp"
#include "LoadBalancer.hpp"
#include "MessageInterface.hpp"

class ChatroomManagerBalancer : public LoadBalancer {
    private:
        virtual std::shared_ptr<Manager> createManager(Server& server) override;
    public:
        using LoadBalancer::LoadBalancer;
};
