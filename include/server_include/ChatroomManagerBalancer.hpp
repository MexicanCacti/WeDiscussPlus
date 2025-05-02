#ifndef CHATROOMMANAGERBALANCER_HPP
#define CHATROOMMANAGERBALANCER_HPP
#include "server_include/LoadBalancer.hpp"
#include "server_include/ChatroomManager.hpp"
#include "shared_include/Message.hpp"
class ChatroomManagerBalancer : public LoadBalancer<Message, ChatroomManager>{

};

#endif