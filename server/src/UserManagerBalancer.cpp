#include "UserManagerBalancer.hpp"

template<typename WorkType>
std::shared_ptr<Manager<WorkType>> UserManagerBalancer<WorkType>::createManager(Server<WorkType>& server){
    return std::make_shared<UserManager<WorkType>>(server);
}

template class UserManagerBalancer<Message>;
template class UserManagerBalancer<MockMessage>;