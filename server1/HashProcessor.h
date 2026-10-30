#pragma once
#include <string>
#include <unordered_map>
#include <cstdint>

class HashProcessor {
public:
    static std::string sha224Hex(const std::string& data);
    static std::string sha224SaltPassword(const uint8_t salt[8], const std::string& password);
    static bool loadDatabase(const std::string& path, std::unordered_map<std::string, std::string>& out);
};