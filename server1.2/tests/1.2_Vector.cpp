#include "test_support.h"
#include "Vector.h"

void test_1_2() {
    CHECK(VectorProcessor::arithmeticMean({}) == 0.0f);
}
