#include "math/common_math.h"

float lerp_f(float a, float b, float t) {
    return (1.0f - t) * a + t * b;
}

float cm_maxf(float a, float b) {
    return a >= b ? a : b;
}
