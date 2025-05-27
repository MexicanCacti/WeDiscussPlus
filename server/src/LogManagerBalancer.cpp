#include "LogManagerBalancer.hpp"

std::shared_ptr<Manager> LogManagerBalancer::createManager(Server& server) {
    return std::make_shared<LogManager>(server);
}