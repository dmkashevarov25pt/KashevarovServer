#include "test_support.h"
#include "HashProcessor.h"

void test_2_5() {
    const std::string salt16 = "0000000000000001";
    const auto correct = HashProcessor::sha224SaltPassword(salt16, "correct-password");
    const auto wrong = HashProcessor::sha224SaltPassword(salt16, "wrong-password");
    CHECK(correct != wrong);
}
