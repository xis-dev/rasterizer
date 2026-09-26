#ifndef MAT3_H
#define MAT3_H

#include "vec3.h"

#include <stddef.h>

typedef struct{
    vec3 c0;
    vec3 c1;
    vec3 c2;

} mat3;

mat3 mat3_construct_fill(float s);

mat3 mat3_construct_diagonal(float s);

mat3 mat3_construct(vec3 _c0, vec3 _c1, vec3 _c2);

mat3 mat3_add(mat3 m0, mat3 m1);

mat3 mat3_add_multiple(size_t count, ...);

void mat3_add_mut(mat3* m0, mat3 m1);

void mat3_add_multiple_mut(mat3* m0, size_t count, ...);

mat3 mat3_sub(mat3 m0, mat3 m1);

mat3 mat3_sub_multiple(size_t count, ...);

void mat3_sub_mut(mat3* m0, mat3 m1);

void mat3_sub_multiple_mut(mat3* m0, size_t count, ...);

mat3 mat3_mult(mat3 m0, mat3 m1);

mat3 mat3_mult_multiple(size_t count, ...);

void mat3_mult_mut(mat3 m, mat3* m_out);

void mat3_mult_multiple_mut(mat3* m0, size_t count, ...);

mat3 mat3_get_transpose(mat3 m);

void mat3_transpose(mat3* m);

mat3 mat3_rotate_by_axis(mat3 m, double angle, vec3 axis);

mat3 mat3_scale_by_axis(mat3 m, double s, vec3 axis);

mat3 mat3_scale_uniform(mat3 m, double s);

mat3 mat3_scale(mat3 m, vec3 scale);

vec3 mat3_transform(mat3 m, vec3 v);

float mat3_determinant(mat3 m);

void mat3_inverse(mat3* m);

mat3 mat3_get_inverse(mat3 m);

void mat3_print(mat3 m);

#endif
