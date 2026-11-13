#include "test_support.h"
#include "cli_test_support.h"

void test_5_1() {
    int status = -1;
    const std::string output = runCli("./vcalc", status);
    CHECK(status == 0);
    CHECK(output.find("Использование: vcalc") != std::string::npos);
    CHECK(output.find("/etc/vcalc.conf") != std::string::npos);
    CHECK(output.find("/var/log/vcalc.log") != std::string::npos);
    CHECK(output.find("33333") != std::string::npos);
}
