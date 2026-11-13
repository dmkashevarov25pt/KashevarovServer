#include "Vector.h"

float VectorProcessor::arithmeticMean(const std::vector<float>& values) {
    if (values.empty()) return 0.0f;

    double sum = 0.0;
    for (float value : values) sum += static_cast<double>(value);
    return static_cast<float>(sum / static_cast<double>(values.size()));
}
