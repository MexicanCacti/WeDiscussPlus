#ifndef LOGMANAGERBALANCER_HPP
#define LOGMANAGERBALANCER_HPP
#include "LogManager.hpp"
#include "LoadBalancer.hpp"
#include "Message.hpp"
#include "shared_include/MockMessage.hpp"

template<typename WorkType>
class LogManagerBalancer : public LoadBalancer<WorkType>{
    private:
        virtual std::shared_ptr<Manager<WorkType>> createManager(Server<WorkType>& server) override;
    public:
        using LoadBalancer<WorkType>::LoadBalancer;
};

#endif