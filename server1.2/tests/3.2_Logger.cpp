#include "test_support.h"
#include "Logger.h"
#include <filesystem>
#include <fstream>
#include <iterator>
#include <regex>

void test_3_2() {
    const auto path = std::filesystem::temp_directory_path() / "vcalc_test_logger_32.log";
    std::filesystem::remove(path);
    auto& logger = Logger::instance();
    logger.init(path.string());
    logger.info("unit-test-info-marker");
    logger.critical("unit-test-critical-marker");

    std::ifstream file(path);
    const std::string contents((std::istreambuf_iterator<char>(file)), {});
    std::filesystem::remove(path);
    CHECK(contents.find("unit-test-info-marker") != std::string::npos);
    CHECK(contents.find("unit-test-critical-marker") != std::string::npos);
    CHECK(contents.find("[INFO]") != std::string::npos);
    CHECK(contents.find("[CRITICAL]") != std::string::npos);
    CHECK(std::regex_search(contents, std::regex(R"(\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2})")));
}
