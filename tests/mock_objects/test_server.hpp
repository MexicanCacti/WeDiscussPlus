#ifndef TEST_SERVER_HPP
#define TEST_SERVER_HPP
#include "server_include/Server.hpp"

class TestingServer : public Server {
    public:
        TestingServer(int port) : Server(port) {}
};

#endif