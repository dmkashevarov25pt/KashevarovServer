#include "test_support.h"
#include "HashProcessor.h"
#include <algorithm>
#include <cctype>

void test_2_3() {
    const std::string salt16 = "0000000000000001";
    const std::string hash = HashProcessor::sha224SaltPassword(salt16, "password");
    CHECK(hash == "cae626eca0813c96efd34b3bc3cc7f37f01417005497c62d1d5b791e");
    CHECK(hash.size() == 56);
    CHECK(std::all_of(hash.begin(), hash.end(), [](unsigned char c) {
        return std::isdigit(c) || (c >= 'a' && c <= 'f');
    }));
}
