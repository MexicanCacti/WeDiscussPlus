#include "ChatroomManagerBalancer.hpp"

std::shared_ptr<Manager> ChatroomManagerBalancer::createManager(Server& server){
    return std::make_shared<ChatroomManager>(server);
}