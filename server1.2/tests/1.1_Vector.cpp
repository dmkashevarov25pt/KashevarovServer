#include "test_support.h"
#include "Vector.h"
#include <vector>

void test_1_1() {
    const std::vector<float> values{1.5f, 2.5f, -1.0f, 4.0f};
    CHECK(std::fabs(VectorProcessor::arithmeticMean(values) - 1.75f) < 0.0001f);
}
