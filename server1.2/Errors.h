#pragma once
#include <stdexcept>
#include <string>

/** Base exception type for server failures. */
class ServerError : public std::runtime_error {
public:
    explicit ServerError(const std::string& message) : std::runtime_error(message) {}
};

/** Invalid startup configuration or unavailable required configuration file. */
class ConfigError : public ServerError {
public:
    explicit ConfigError(const std::string& message)
        : ServerError("Configuration error: " + message) {}
};

/** Socket or network communication failure. */
class NetworkError : public ServerError {
public:
    explicit NetworkError(const std::string& message)
        : ServerError("Network error: " + message) {}
};

/** Client authentication failure. */
class AuthError : public ServerError {
public:
    explicit AuthError(const std::string& message)
        : ServerError("Authentication error: " + message) {}
};

/** Malformed or unsupported client protocol data. */
class ProtocolError : public ServerError {
public:
    explicit ProtocolError(const std::string& message)
        : ServerError("Protocol error: " + message) {}
};
