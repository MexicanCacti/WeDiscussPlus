#pragma once
#include "UserManager.hpp"
#include "LoadBalancer.hpp"
#include "MessageInterface.hpp"

class UserManagerBalancer : public LoadBalancer{
    private:
        virtual std::shared_ptr<Manager> createManager(Server& server) override;
    public:
        using LoadBalancer::LoadBalancer;
};