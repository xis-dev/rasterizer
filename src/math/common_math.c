#include "common_math.h"

float lerp_f(float a, float b, float t) {
    return (1.0f - t) * a + t * b;
}
