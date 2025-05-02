#ifndef LOGMANAGERBALANCER_HPP
#define LOGMANAGERBALANCER_HPP
#include "server_include/LoadBalancer.hpp"
#include "server_include/LogManager.hpp"
#include "shared_include/Message.hpp"
class LogManagerBalancer : public LoadBalancer<Message, LogManager>{

};

#endif