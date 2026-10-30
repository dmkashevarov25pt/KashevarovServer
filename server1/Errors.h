#pragma once
#include <stdexcept>
#include <string>

class ServerError : public std::runtime_error {
public:
    explicit ServerError(const std::string& msg) : std::runtime_error(msg) {}
};

class ConfigError : public ServerError {
public:
    explicit ConfigError(const std::string& msg) : ServerError("Ошибка конфигурации: " + msg) {}
};

class NetworkError : public ServerError {
public:
    explicit NetworkError(const std::string& msg) : ServerError("Сетевая ошибка: " + msg) {}
};

class AuthError : public ServerError {
public:
    explicit AuthError(const std::string& msg) : ServerError("Ошибка авторизации: " + msg) {}
};

class ProtocolError : public ServerError {
public:
    explicit ProtocolError(const std::string& msg) : ServerError("Ошибка протокола: " + msg) {}
};