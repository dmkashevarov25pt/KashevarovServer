#pragma once
#include <string>
#include <cstdint>
#include <cstddef>

class Communicator {
public:
    Communicator(int fd, std::string login);
    ~Communicator();

    void run();

private:
    void sendRaw(const void* buf, size_t n);
    void recvRaw(void* buf, size_t n);

    void sendLine(const std::string& s);
    std::string recvLine();

    bool authenticate();
    void processVectors();

    int         fd_;
    std::string login_;
    bool        authed_ = false;
};