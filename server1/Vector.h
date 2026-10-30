#pragma once
#include <vector>
#include <cstdint>
#include <cfloat>
#include <cmath>

class VectorProcessor {
public:
    static float product(const std::vector<float>& v);
    static float saturate(float x);
};