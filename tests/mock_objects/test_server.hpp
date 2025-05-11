#ifndef TEST_SERVER_HPP
#define TEST_SERVER_HPP
#include "Server.hpp"
#include "MockMessage.hpp"
class TestingServer : public Server<MockMessage> {
    public:
        TestingServer(int port) : Server<MockMessage>("127.0.0.1", port) {}
};

#endif