#include "test_support.h"
#include "cli_test_support.h"

void test_5_2() {
    int status = -1;
    const std::string output = runCli("./vcalc -h", status);
    CHECK(status == 0);
    CHECK(output.find("Использование: vcalc") != std::string::npos);
    CHECK(output.find("Параметры:") != std::string::npos);
}
