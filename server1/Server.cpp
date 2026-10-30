#include "Server.h"
#include "Communicator.h"
#include "HashProcessor.h"
#include "Logger.h"
#include "Errors.h"

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <unordered_map>

std::unordered_map<std::string, std::string>* g_users = nullptr;
static std::unordered_map<std::string, std::string> g_userDb;

Server::Server(uint16_t port, std::string dbPath, std::string logPath)
    : port_(port), dbPath_(std::move(dbPath)), logPath_(std::move(logPath)) {}

Server::~Server() {
    if (listenFd_ >= 0) ::close(listenFd_);
}

void Server::setupSocket() {
    listenFd_ = ::socket(AF_INET, SOCK_STREAM, 0);
    if (listenFd_ < 0) throw NetworkError("Не удалось создать сокет");

    int one = 1;
    ::setsockopt(listenFd_, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));

    sockaddr_in addr{};
    addr.sin_family      = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port        = htons(port_);

    if (::bind(listenFd_, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0)
        throw NetworkError("Не удалось привязать сокет к порту " + std::to_string(port_));

    if (::listen(listenFd_, 16) < 0)
        throw NetworkError("Не удалось перевести сокет в режим прослушивания");
}

void Server::run() {
    Logger::instance().init(logPath_);

    if (!HashProcessor::loadDatabase(dbPath_, g_userDb)) {
        Logger::instance().critical("Невозможно загрузить базу данных " + dbPath_);
        throw ConfigError("Файл базы данных не найден: " + dbPath_);
    }
    g_users = &g_userDb;

    setupSocket();
    Logger::instance().info("Прослушивание порта " + std::to_string(port_));

    while (true) {
        sockaddr_in cli{};
        socklen_t len = sizeof(cli);
        int fd = ::accept(listenFd_, reinterpret_cast<sockaddr*>(&cli), &len);
        if (fd < 0) {
            Logger::instance().warn("Ошибка при принятии соединения (accept)");
            continue;
        }

        char ip[INET_ADDRSTRLEN] = {0};
        inet_ntop(AF_INET, &cli.sin_addr, ip, sizeof(ip));
        Logger::instance().info(std::string("Подключился клиент с IP: ") + ip);

        try {
            Communicator comm(fd, "");
            comm.run();
        } catch (const std::exception& e) {
            Logger::instance().warn(std::string("Сессия прервана: ") + e.what());
        }
    }
}