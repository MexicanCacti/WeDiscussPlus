#ifndef USERMANAGERBALANCER_HPP
#define USERMANAGERBALANCER_HPP
#include "server_include/UserManager.hpp"
#include "server_include/LoadBalancer.hpp"
#include "shared_include/Message.hpp"

class UserManagerBalancer : public LoadBalancer<Message, UserManager<Message>>{
    public:
        UserManagerBalancer(const int workerAmount)
            : LoadBalancer<Message, UserManager<Message>>(workerAmount) {}
};

#endif