#pragma once

#include <cstdio>
#include <stdexcept>
#include <string>
#include <sys/wait.h>

inline std::string runCli(const char* command, int& exitCode) {
    FILE* pipe = ::popen(command, "r");
    if (!pipe) throw std::runtime_error("could not start vcalc");
    std::string output;
    char buffer[256];
    while (std::fgets(buffer, sizeof(buffer), pipe)) output += buffer;
    const int status = ::pclose(pipe);
    if (status == -1 || !WIFEXITED(status)) throw std::runtime_error("vcalc did not exit normally");
    exitCode = WEXITSTATUS(status);
    return output;
}
