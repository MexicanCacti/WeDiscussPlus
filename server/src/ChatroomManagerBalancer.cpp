#include "server_include/ChatroomManagerBalancer.hpp"

template<typename WorkType>
std::shared_ptr<Manager<WorkType>> ChatroomManagerBalancer<WorkType>::createManager(Server& server){
    return std::make_shared<ChatroomManager<WorkType>>(server);
}
    
template class ChatroomManagerBalancer<Message>;