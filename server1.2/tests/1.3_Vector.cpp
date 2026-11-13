#include "test_support.h"
#include "Vector.h"

void test_1_3() {
    CHECK(VectorProcessor::arithmeticMean({-42.5f}) == -42.5f);
}
