#pragma once
#include <vector>
#include <cstdint>

/** Performs calculations on vectors received from clients. */
class VectorProcessor {
public:
    /** Return the arithmetic mean; an empty vector produces 0.0f. */
    static float arithmeticMean(const std::vector<float>& v);
};
