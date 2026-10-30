#include "HashProcessor.h"
#include "Logger.h"
#include <openssl/sha.h>
#include <fstream>
#include <sstream>
#include <iomanip>

std::string HashProcessor::sha224Hex(const std::string& data) {
    unsigned char digest[SHA224_DIGEST_LENGTH];
    SHA224(reinterpret_cast<const unsigned char*>(data.data()), data.size(), digest);

    std::ostringstream oss;
    for (int i = 0; i < SHA224_DIGEST_LENGTH; ++i)
        oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<unsigned>(digest[i]);
    return oss.str();
}

std::string HashProcessor::sha224SaltPassword(const uint8_t salt[8], const std::string& password) {
    std::string buf;
    buf.reserve(8 + password.size());
    buf.append(reinterpret_cast<const char*>(salt), 8);
    buf.append(password);
    return sha224Hex(buf);
}

bool HashProcessor::loadDatabase(const std::string& path, std::unordered_map<std::string, std::string>& out) {
    std::ifstream in(path);
    if (!in.is_open()) {
        Logger::instance().critical("Невозможно открыть базу данных пользователей: " + path);
        return false;
    }

    std::string line;
    size_t n = 0;
    while (std::getline(in, line)) {
        if (line.empty() || line[0] == '#') continue;

        auto pos = line.find(':');
        if (pos == std::string::npos) {
            Logger::instance().warn("Некорректная строка в базе данных (отсутствует ':'): " + line);
            continue;
        }
        out[line.substr(0, pos)] = line.substr(pos + 1);
        ++n;
    }
    Logger::instance().info("База данных успешно загружена. Пользователей: " + std::to_string(n));
    return true;
}