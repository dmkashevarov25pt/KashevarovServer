#include "test_support.h"
#include "communicator_test_support.h"
#include "HashProcessor.h"
#include <algorithm>
#include <cctype>
#include <unordered_map>

void test_4_1() {
    std::unordered_map<std::string, std::string> users{{"user", "password"}};
    LocalCommunicator client(users);
    client.sendAll("user\r\n", 6);

    const std::string salt16 = client.recvLine();
    CHECK(salt16.size() == 16);
    CHECK(std::all_of(salt16.begin(), salt16.end(), [](unsigned char c) {
        return std::isdigit(c) || (c >= 'A' && c <= 'F');
    }));

    std::string hash = HashProcessor::sha224SaltPassword(salt16, "password");
    std::transform(hash.begin(), hash.end(), hash.begin(),
                   [](unsigned char c) { return static_cast<char>(std::toupper(c)); });
    hash += '\n';
    client.sendAll(hash.data(), hash.size());

    const uint32_t vectorCount = 1;
    const uint32_t vectorSize = 1;
    const float value = 42.0f;
    client.sendAll(&vectorCount, sizeof(vectorCount));
    client.sendAll(&vectorSize, sizeof(vectorSize));
    client.sendAll(&value, sizeof(value));
    uint32_t resultCount = 0;
    float result = 0.0f;
    client.recvAll(&resultCount, sizeof(resultCount));
    client.recvAll(&result, sizeof(result));
    CHECK(resultCount == 1);
    CHECK(result == value);
}
