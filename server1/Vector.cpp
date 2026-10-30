#include "Vector.h"

float VectorProcessor::saturate(float x) {
    if (std::isinf(x) || std::isnan(x))
        return (x < 0) ? -FLT_MAX : FLT_MAX;
    if (x >  FLT_MAX) return  FLT_MAX;
    if (x < -FLT_MAX) return -FLT_MAX;
    return x;
}

float VectorProcessor::product(const std::vector<float>& v) {
    if (v.empty()) return 0.0f;

    float p = 1.0f;
    for (float x : v) {
        double d = static_cast<double>(p) * static_cast<double>(x);
        if (d >  static_cast<double>(FLT_MAX)) return  FLT_MAX;
        if (d < -static_cast<double>(FLT_MAX)) return -FLT_MAX;
        p = static_cast<float>(d);
    }
    return p;
}