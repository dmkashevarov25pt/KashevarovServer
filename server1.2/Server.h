#pragma once
#include <string>
#include <cstdint>

/** Runs the single-threaded TCP server and loads its user database at startup. */
class Server {
public:
    /** Create a server using the selected listening port, database, and log paths. */
    Server(uint16_t port, std::string dbPath, std::string logPath);
    ~Server();

    /** Load configuration data, listen for clients, and process sessions sequentially. */
    void run();

private:
    void setupSocket();

    uint16_t    port_;
    std::string dbPath_;
    std::string logPath_;
    int         listenFd_ = -1;
};
