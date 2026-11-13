#include "test_support.h"
#include "HashProcessor.h"
#include <filesystem>
#include <fstream>
#include <unordered_map>

void test_2_4() {
    const auto path = std::filesystem::temp_directory_path() / "vcalc_test_crlf_db_24.conf";
    { std::ofstream file(path, std::ios::binary | std::ios::trunc); file << "admin:secret\r\n"; }
    std::unordered_map<std::string, std::string> users;
    const bool loaded = HashProcessor::loadDatabase(path.string(), users);
    std::filesystem::remove(path);
    CHECK(loaded);
    CHECK(users.at("admin") == "secret");
}
