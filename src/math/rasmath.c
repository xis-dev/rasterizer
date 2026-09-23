#include "rasmath.h"

vec3 vec3_convert_vec2(vec2 v, float z) {
    return (vec3){v.e[0], v.e[1], z};
}

vec4 vec4_convert_vec2(vec2 v, float z, float w) {
    return (vec4){v.e[0], v.e[1], z, w};
}

vec2 vec2_convert_vec3(vec3 v) {
    return (vec2){v.e[0], v.e[1]};
}

vec4 vec4_convert_vec3(vec3 v, float w) {
    return (vec4){v.e[0], v.e[1], v.e[2], w};
}

vec2 vec2_convert_vec4(vec4 v) {
    return (vec2){v.e[0], v.e[1]};
}

vec3 vec3_convert_vec4(vec4 v) {
    return (vec3){v.e[0], v.e[1], v.e[2]};
}

mat3 mat3_convert_mat4(mat4 m) {
    return (mat3){.c0 = vec3_convert_vec4(m.c0), .c1 = vec3_convert_vec4(m.c1), .c2 = vec3_convert_vec4(m.c2)};
}

mat4 mat4_convert_mat3(mat3 m, vec4 column_3, vec3 row_3) {

    return (mat4){.c0 = vec4_convert_vec3(m.c0, row_3.e[0]),
                  .c1 = vec4_convert_vec3(m.c1, row_3.e[1]),
                  .c2 = vec4_convert_vec3(m.c2, row_3.e[2]),
                  .c3 = column_3};
}
