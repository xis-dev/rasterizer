#ifndef SHADER_VALUE_H
#define SHADER_VALUE_H

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "rasmath.h"

typedef enum {UTYPE_NULL, UTYPE_INT32, UTYPE_UINT32, UTYPE_F32, UTYPE_VEC2_F32, UTYPE_VEC3_F32, UTYPE_VEC4_F32, UTYPE_MAT4_F32} u_type;

typedef struct {
    void* value;
    u_type value_type;
} shader_value;

#define CTYPE_TO_UTYPE(c_type_val) \
        _Generic((c_type_val), \
        int32_t: UTYPE_INT32, \
        uint32_t: UTYPE_UINT32, \
        float: UTYPE_F32, \
        vec2: UTYPE_VEC2_F32, \
        vec3: UTYPE_VEC3_F32, \
        vec4: UTYPE_VEC4_F32, \
        mat4: UTYPE_MAT4_F32 \
)

#define sv_create(val, type, sv_out) \
    do { \
    _Static_assert(__builtin_types_compatible_p(typeof(val), type), "SHADER_VALUE: Found mismatched types between provided value and provided type while creating shader value");\
    (sv_out)->value_type = CTYPE_TO_UTYPE(val); \
    (sv_out)->value = malloc(sizeof(type)); \
    *((type*)(sv_out)->value) = val; \
} while (0)

#define sv_get_value(sv, out_ptr) \
    do { \
    if ((sv).value_type != CTYPE_TO_UTYPE(*(out_ptr))) {\
        printf("SHADER_VALUE: Found mismatched types beteween provided output type and stored shader value type, while trying to get shader value"); \
        break; \
        }\
    memcpy((out_ptr), (sv).value, sizeof(*(out_ptr))); \
} while (0)

#define sv_change_value(sv_ptr, val) \
    do {\
    if ((sv_ptr)->value_type != CTYPE_TO_UTYPE((val))) {\
    printf("SHADER_VALUE: Found mismatched types beteween provided value type and stored shader value type, while trying to change shader value"); \
    break; \
    } \
    memcpy((sv_ptr)->value, &(val), sizeof((val))); \
} while (0)

#define sv_destroy(sv_ptr) \
    do {\
    free((sv_ptr)->value);\
    (sv_ptr)->value_type = UTYPE_NULL; \
} while(0)

#endif