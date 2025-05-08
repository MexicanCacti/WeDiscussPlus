#ifndef LOGMANAGERBALANCER_HPP
#define LOGMANAGERBALANCER_HPP
#include "server_include/LogManager.hpp"
#include "server_include/LoadBalancer.hpp"
#include "shared_include/Message.hpp"

template<typename WorkType>
class LogManagerBalancer : public LoadBalancer<WorkType>{
    private:
        virtual std::shared_ptr<Manager<WorkType>> createManager(Server& server) override;
    public:
        using LoadBalancer<WorkType>::LoadBalancer;
};

#endif