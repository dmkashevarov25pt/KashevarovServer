#pragma once
#include <string>
#include <cstdint>

class Server {
public:
    Server(uint16_t port, std::string dbPath, std::string logPath);
    ~Server();

    void run();

private:
    void setupSocket();

    uint16_t    port_;
    std::string dbPath_;
    std::string logPath_;
    int         listenFd_ = -1;
};