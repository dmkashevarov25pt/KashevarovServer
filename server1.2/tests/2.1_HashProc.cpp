#include "test_support.h"
#include "HashProcessor.h"
#include <filesystem>
#include <unordered_map>

void test_2_1() {
    const auto path = std::filesystem::temp_directory_path() / "vcalc_test_missing_db_21.conf";
    std::error_code ec;
    std::filesystem::remove(path, ec);
    std::unordered_map<std::string, std::string> users;
    CHECK(!HashProcessor::loadDatabase(path.string(), users));
}
