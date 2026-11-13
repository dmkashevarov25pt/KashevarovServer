#pragma once
#include <string>
#include <cstdint>
#include <cstddef>

/** Handles one client's text authentication and binary vector session over a TCP socket. */
class Communicator {
public:
    /** Take ownership of an accepted socket and handle the client's session. */
    Communicator(int fd, std::string login);
    ~Communicator();

    /** Authenticate the client, process its vectors, and close the session on completion. */
    void run();

private:
    void sendRaw(const void* buf, size_t n);
    void recvRaw(void* buf, size_t n);

    void sendLine(const std::string& s);
    std::string recvLine();

    bool authenticate(const std::string& login);
    void processVectors();

    int         fd_;
    std::string login_;
    bool        authed_ = false;
};
