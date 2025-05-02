#ifndef USERMANAGERBALANCER_HPP
#define USERMANAGERBALANCER_HPP

#include "server_include/LoadBalancer.hpp"
#include "server_include/UserManager.hpp"
#include "shared_include/Message.hpp"

class UserManagerBalancer : public LoadBalancer<Message, UserManager<Message>>{
    
};

#endif