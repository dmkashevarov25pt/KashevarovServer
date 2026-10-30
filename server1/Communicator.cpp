#include "Communicator.h"
#include "HashProcessor.h"
#include "Vector.h"
#include "Logger.h"
#include "Errors.h"

#include <sys/socket.h>
#include <unistd.h>
#include <random>
#include <cstring>
#include <vector>
#include <cstdint>
#include <unordered_map>

extern std::unordered_map<std::string, std::string>* g_users;

static constexpr size_t   MAX_LINE    = 1024;
static constexpr uint32_t MAX_VECTORS = 1'000'000;
static constexpr uint32_t MAX_SIZE    = 100'000'000;

Communicator::Communicator(int fd, std::string login)
    : fd_(fd), login_(std::move(login)) {}

Communicator::~Communicator() {
    if (fd_ >= 0) ::close(fd_);
}

void Communicator::sendRaw(const void* buf, size_t n) {
    const char* p    = static_cast<const char*>(buf);
    size_t      left = n;
    while (left > 0) {
        ssize_t r = ::send(fd_, p, left, MSG_NOSIGNAL);
        if (r <= 0) throw NetworkError("Ошибка отправки данных (send)");
        p    += r;
        left -= static_cast<size_t>(r);
    }
}

void Communicator::recvRaw(void* buf, size_t n) {
    char*  p    = static_cast<char*>(buf);
    size_t left = n;
    while (left > 0) {
        ssize_t r = ::recv(fd_, p, left, 0);
        if (r <= 0) throw NetworkError("Ошибка приема данных (recv) или соединение разорвано");
        p    += r;
        left -= static_cast<size_t>(r);
    }
}

void Communicator::sendLine(const std::string& s) {
    std::string tmp = s + "\n";
    sendRaw(tmp.data(), tmp.size());
}

std::string Communicator::recvLine() {
    std::string line;
    line.reserve(64);
    char c;
    while (line.size() < MAX_LINE) {
        recvRaw(&c, 1);
        if (c == '\n') break;
        if (c != '\r') line.push_back(c);
    }
    return line;
}

bool Communicator::authenticate() {
    std::string login = recvLine();
    if (login.empty()) {
        sendLine("ERR");
        Logger::instance().warn("Пустой логин от клиента");
        return false;
    }
    login_ = login;

    if (!g_users || g_users->find(login_) == g_users->end()) {
        sendLine("ERR");
        Logger::instance().warn("Неизвестный логин: " + login_);
        return false;
    }

    uint8_t salt[8];
    std::random_device rd;
    for (int i = 0; i < 8; i += 4) {
        uint32_t r = rd();
        std::memcpy(salt + i, &r, 4);
    }
    sendRaw(salt, 8);

    std::string clientHash = recvLine();
    if (clientHash.empty()) {
        sendLine("ERR");
        return false;
    }

    std::string expected = HashProcessor::sha224SaltPassword(salt, (*g_users)[login_]);

    if (clientHash != expected) {
        sendLine("ERR");
        Logger::instance().warn("Ошибка аутентификации для пользователя " + login_);
        return false;
    }

    sendLine("OK");
    Logger::instance().info("Успешная аутентификация: " + login_);
    return true;
}

void Communicator::processVectors() {
    uint32_t n = 0;
    recvRaw(&n, 4);
    if (n == 0 || n > MAX_VECTORS)
        throw ProtocolError("Некорректное количество векторов: " + std::to_string(n));

    for (uint32_t i = 0; i < n; ++i) {
        uint32_t sz = 0;
        recvRaw(&sz, 4);
        if (sz == 0 || sz > MAX_SIZE)
            throw ProtocolError("Некорректный размер вектора: " + std::to_string(sz));

        std::vector<float> vec(sz);
        recvRaw(vec.data(), sz * sizeof(float));

        float res = VectorProcessor::product(vec);
        uint32_t cnt = 1;
        sendRaw(&cnt, 4);
        sendRaw(&res, 4);
    }
}

void Communicator::run() {
    try {
        if (!authenticate()) return;
        processVectors();
        Logger::instance().info("Сессия успешно завершена для " + login_);
    } catch (const std::exception& e) {
        Logger::instance().warn(std::string("Ошибка сессии: ") + e.what());
    }
}