#include "test_support.h"
#include "communicator_test_support.h"
#include <unordered_map>

void test_4_2() {
    std::unordered_map<std::string, std::string> users{{"user", "password"}};
    LocalCommunicator client(users);
    const std::string unknownLogin = "unknown_user\n";
    client.sendAll(unknownLogin.data(), unknownLogin.size());
    CHECK(client.recvLine() == "ERR");
    CHECK(client.peerClosed());
}
