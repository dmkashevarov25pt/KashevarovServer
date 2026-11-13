#include "test_support.h"
#include "communicator_test_support.h"
#include "HashProcessor.h"
#include <string>
#include <unordered_map>

void test_4_4() {
    std::unordered_map<std::string, std::string> users{{"user", "password"}};
    LocalCommunicator client(users);
    client.sendAll("user\n", 5);
    const std::string salt16 = client.recvLine();
    const std::string wrongHash = HashProcessor::sha224SaltPassword(salt16, "wrong-password") + "\n";
    client.sendAll(wrongHash.data(), wrongHash.size());
    CHECK(client.recvLine() == "ERR");
    CHECK(client.peerClosed());
}
