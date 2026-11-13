#include "test_support.h"
#include "HashProcessor.h"
#include <filesystem>
#include <fstream>
#include <unordered_map>

void test_2_2() {
    const auto path = std::filesystem::temp_directory_path() / "vcalc_test_empty_db_22.conf";
    { std::ofstream file(path, std::ios::trunc); CHECK(file.is_open()); }
    std::unordered_map<std::string, std::string> users;
    const bool loaded = HashProcessor::loadDatabase(path.string(), users);
    std::filesystem::remove(path);
    CHECK(loaded);
    CHECK(users.empty());
}
