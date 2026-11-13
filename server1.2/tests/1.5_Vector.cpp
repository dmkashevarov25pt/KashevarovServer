#include "test_support.h"
#include "Vector.h"
#include <cmath>

void test_1_5() {
    const float result = VectorProcessor::arithmeticMean({-3.4e38f, -3.4e38f});
    CHECK(std::isfinite(result));
    CHECK(std::fabs(result - (-3.4e38f)) < 1.0e32f);
}
