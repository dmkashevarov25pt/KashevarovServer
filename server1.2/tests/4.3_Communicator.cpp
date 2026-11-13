#include "test_support.h"
#include "communicator_test_support.h"
#include "HashProcessor.h"
#include <cmath>
#include <cstdint>
#include <string>
#include <unordered_map>

void test_4_3() {
    std::unordered_map<std::string, std::string> users{{"user", "password"}};
    LocalCommunicator client(users);
    client.sendAll("user\n", 5);
    const std::string salt16 = client.recvLine();
    const std::string hash = HashProcessor::sha224SaltPassword(salt16, "password") + "\n";
    client.sendAll(hash.data(), hash.size());
    const uint32_t vectorCount = 1;
    const uint32_t vectorSize = 3;
    const float values[3]{1.5f, 2.5f, 3.0f};
    client.sendAll(&vectorCount, sizeof(vectorCount));
    client.sendAll(&vectorSize, sizeof(vectorSize));
    client.sendAll(values, sizeof(values));

    uint32_t resultCount = 0;
    float result = 0.0f;
    client.recvAll(&resultCount, sizeof(resultCount));
    client.recvAll(&result, sizeof(result));
    CHECK(resultCount == 1);
    CHECK(std::fabs(result - (7.0f / 3.0f)) < 0.0001f);
}
