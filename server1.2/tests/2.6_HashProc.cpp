#include "test_support.h"
#include "HashProcessor.h"
#include <filesystem>
#include <fstream>
#include <unordered_map>

void test_2_6() {
    const auto path = std::filesystem::temp_directory_path() / "vcalc_test_users_26.conf";
    { std::ofstream file(path, std::ios::trunc); file << "user:stored-hash\n"; }
    std::unordered_map<std::string, std::string> users;
    const bool loaded = HashProcessor::loadDatabase(path.string(), users);
    std::filesystem::remove(path);
    CHECK(loaded);
    CHECK(users.count("unknown_user") == 0);
}
