#pragma once
#include <string>
#include <unordered_map>

/** Provides SHA-224 helpers and reads login/password pairs from a text database. */
class HashProcessor {
public:
    /** Return the lowercase hexadecimal SHA-224 digest of data. */
    static std::string sha224Hex(const std::string& data);

    /** Hash the ASCII SALT16 string concatenated with the user's password. */
    static std::string sha224SaltPassword(const std::string& salt16, const std::string& password);

    /** Load login:password records. Return false if the file cannot be opened. */
    static bool loadDatabase(const std::string& path, std::unordered_map<std::string, std::string>& out);
};
