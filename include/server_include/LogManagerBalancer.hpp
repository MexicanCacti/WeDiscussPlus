#pragma once
#include "LogManager.hpp"
#include "LoadBalancer.hpp"
#include "MessageInterface.hpp"

class LogManagerBalancer : public LoadBalancer{
    private:
        virtual std::shared_ptr<Manager> createManager(Server& server) override;
    public:
        using LoadBalancer::LoadBalancer;
};
