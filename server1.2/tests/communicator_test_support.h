#pragma once

#include "Communicator.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdexcept>
#include <string>
#include <thread>
#include <unordered_map>

extern std::unordered_map<std::string, std::string>* g_users;

class LocalCommunicator {
public:
    explicit LocalCommunicator(std::unordered_map<std::string, std::string>& users) {
        const int listener = ::socket(AF_INET, SOCK_STREAM, 0);
        if (listener < 0) throw std::runtime_error("TCP listener socket failed");
        sockaddr_in address{};
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
        address.sin_port = 0;
        if (::bind(listener, reinterpret_cast<sockaddr*>(&address), sizeof(address)) != 0 ||
            ::listen(listener, 1) != 0) {
            ::close(listener);
            throw std::runtime_error("TCP listener setup failed");
        }
        socklen_t addressSize = sizeof(address);
        if (::getsockname(listener, reinterpret_cast<sockaddr*>(&address), &addressSize) != 0) {
            ::close(listener);
            throw std::runtime_error("getsockname failed");
        }
        client_ = ::socket(AF_INET, SOCK_STREAM, 0);
        if (client_ < 0 || ::connect(client_, reinterpret_cast<sockaddr*>(&address), sizeof(address)) != 0) {
            if (client_ >= 0) ::close(client_);
            ::close(listener);
            throw std::runtime_error("TCP client connection failed");
        }
        const int serverFd = ::accept(listener, nullptr, nullptr);
        ::close(listener);
        if (serverFd < 0) {
            ::close(client_);
            client_ = -1;
            throw std::runtime_error("TCP accept failed");
        }
        g_users = &users;
        timeval timeout{5, 0};
        ::setsockopt(client_, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
        server_ = std::thread([serverFd] { Communicator(serverFd, "").run(); });
    }

    LocalCommunicator(const LocalCommunicator&) = delete;
    LocalCommunicator& operator=(const LocalCommunicator&) = delete;

    ~LocalCommunicator() {
        if (client_ >= 0) ::shutdown(client_, SHUT_RDWR);
        if (server_.joinable()) server_.join();
        if (client_ >= 0) ::close(client_);
        g_users = nullptr;
    }

    void sendAll(const void* data, size_t size) {
        const char* p = static_cast<const char*>(data);
        while (size) {
            const ssize_t n = ::send(client_, p, size, 0);
            if (n <= 0) throw std::runtime_error("socket send failed");
            p += n;
            size -= static_cast<size_t>(n);
        }
    }

    void recvAll(void* data, size_t size) {
        char* p = static_cast<char*>(data);
        while (size) {
            const ssize_t n = ::recv(client_, p, size, 0);
            if (n <= 0) throw std::runtime_error("socket receive failed");
            p += n;
            size -= static_cast<size_t>(n);
        }
    }

    std::string recvLine() {
        std::string line;
        char c = 0;
        while (line.size() < 1024) {
            recvAll(&c, 1);
            if (c == '\n') return line;
            if (c != '\r') line.push_back(c);
        }
        throw std::runtime_error("protocol line is too long");
    }

    bool peerClosed() {
        char c;
        return ::recv(client_, &c, 1, 0) == 0;
    }

private:
    int client_ = -1;
    std::thread server_;
};

