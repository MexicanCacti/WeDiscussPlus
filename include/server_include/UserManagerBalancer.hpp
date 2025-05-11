#ifndef USERMANAGERBALANCER_HPP
#define USERMANAGERBALANCER_HPP
#include "UserManager.hpp"
#include "LoadBalancer.hpp"
#include "Message.hpp"
#include "MockMessage.hpp"

template<typename WorkType>
class UserManagerBalancer : public LoadBalancer<WorkType>{
    private:
        virtual std::shared_ptr<Manager<WorkType>> createManager(Server<WorkType>& server) override;
    public:
        using LoadBalancer<WorkType>::LoadBalancer;
};

#endif