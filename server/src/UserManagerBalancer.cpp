#include "UserManagerBalancer.hpp"
#include "Server.hpp"

std::shared_ptr<Manager> UserManagerBalancer::createManager(Server& server){
    return std::make_shared<UserManager>(server);
}