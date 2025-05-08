#include "server_include/LogManagerBalancer.hpp"

template<typename WorkType>
std::shared_ptr<Manager<WorkType>> LogManagerBalancer<WorkType>::createManager(Server& server){
    return std::make_shared<LogManager<WorkType>>(server);
}

template class LogManagerBalancer<Message>;
