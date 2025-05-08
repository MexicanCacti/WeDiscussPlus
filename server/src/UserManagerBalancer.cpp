#include "server_include/UserManagerBalancer.hpp"

template<typename WorkType>
std::shared_ptr<Manager<WorkType>> UserManagerBalancer<WorkType>::createManager(Server& server){
    return std::make_shared<UserManager<WorkType>>(server);
}

template class UserManagerBalancer<Message>;
