#ifndef MAT4_H
#define MAT4_H

#include "vec4.h"
#include "vec3.h"

#include <stddef.h>

typedef struct{
	vec4 c0;
	vec4 c1;
	vec4 c2;
	vec4 c3;

} mat4;

mat4 mat4_construct_fill(float s);

mat4 mat4_construct_diagonal(float s);

mat4 mat4_construct(vec4 _c0, vec4 _c1, vec4 _c2, vec4 _c3);

mat4 mat4_add(mat4 m0, mat4 m1);

mat4 mat4_add_multiple(size_t count, ...);

void mat4_add_mut(mat4* m0, mat4 m1);

void mat4_add_multiple_mut(mat4* m0, size_t count, ...);

mat4 mat4_sub(mat4 m0, mat4 m1);

mat4 mat4_sub_multiple(size_t count, ...);

void mat4_sub_mut(mat4* m0, mat4 m1);

void mat4_sub_multiple_mut(mat4* m0, size_t count, ...);

mat4 mat4_mult(mat4 m0, mat4 m1);

mat4 mat4_mult_multiple(size_t count, ...);

void mat4_mult_mut(mat4 m, mat4* m_out);

void mat4_mult_multiple_mut(mat4* m0, size_t count, ...);

mat4 mat4_get_transpose(mat4 m);

void mat4_transpose(mat4* m);

mat4 mat4_rotate_by_axis(mat4 m, double angle, vec3 axis);

mat4 mat4_scale_by_axis(mat4 m, double s, vec3 axis);

mat4 mat4_translate(mat4 m, vec3 translation);

mat4 mat4_scale_uniform(mat4 m, double s);

mat4 mat4_scale(mat4 m, vec3 scale);

vec4 mat4_transform(mat4 m, vec4 v);

mat4 mat4_lookat(vec3 eye, vec3 target);

mat4 mat4_projection(float near, float far, float fovy, float aspect);

void mat4_print(mat4 m);

#endif