#ifndef CHATROOMMANAGERBALANCER_HPP
#define CHATROOMMANAGERBALANCER_HPP
#include "server_include/ChatroomManager.hpp"
#include "server_include/LoadBalancer.hpp"
#include "shared_include/Message.hpp"

class ChatroomManagerBalancer : public LoadBalancer<Message, ChatroomManager<Message>>{
    public:
        ChatroomManagerBalancer(const int workerAmount)
                : LoadBalancer<Message, ChatroomManager<Message>>(workerAmount) {}
};

#endif