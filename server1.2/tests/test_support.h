#pragma once

#include <cmath>
#include <stdexcept>
#include <string>

inline void check(bool condition, const char* expression) {
    if (!condition) throw std::runtime_error(std::string("Проверка не прошла: ") + expression);
}

#define CHECK(expression) check(static_cast<bool>(expression), #expression)

