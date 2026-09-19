#include "rasmath.h"

vec3 vec3_convert_vec2(vec2 v, float z) {
    return (vec3){v.x, v.y, z};
}

vec4 vec4_convert_vec2(vec2 v, float z, float w) {
    return (vec4){v.x, v.y, z, w};
}

vec2 vec2_convert_vec3(vec3 v) {
    return (vec2){v.x, v.y};
}

vec4 vec4_convert_vec3(vec3 v, float w) {
    return (vec4){v.x, v.y, v.z, w};
}

vec2 vec2_convert_vec4(vec4 v) {
    return (vec2){v.x, v.y};
}

vec3 vec3_convert_vec4(vec4 v) {
    return (vec3){v.x, v.y, v.z};
}
