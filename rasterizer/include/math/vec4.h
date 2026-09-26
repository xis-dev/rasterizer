#ifndef VEC4_H
#define VEC4_H

#include <stdbool.h>

/**
 * Structure of elements for a floating point 3D Vector
 * trailing '_mut' signifies mutating the first vector passed in
 * '_sep' signifies a vector is required but the expected arguments are seperated into its individual x,y, z components
*/
typedef struct vec4 {
    float e[4];
} vec4;

typedef vec4 colour4;
typedef vec4 point4;

// Initialize vector with the same scalar value in all elements
vec4 vec4_construct(float s);
vec4 vec4_construct_sep(float x, float y, float z, float w);
void vec4_construct_mut(vec4* v, float s);
void vec4_construct_sep_mut(vec4* v, float x, float y, float z);

float vec4_magnitude(vec4 v1);

vec4 vec4_zero();

vec4 vec4_add(vec4 v1, vec4 v2);
void vec4_add_mut(vec4* v1, vec4 v2);

vec4 vec4_sub(vec4 v1, vec4 v2);
void vec4_sub_mut(vec4* v1, vec4 v2);

vec4 vec4_scale(vec4 v1, float s);
void vec4_scale_mut(vec4* v1, float s);

// mutl & div define multiplication and division for each element of a vector
vec4 vec4_multiply(vec4 v1, vec4 v2);
void vec4_multiply_mut(vec4* v1, vec4 v2);

vec4 vec4_div(vec4 v1, vec4 v2);
void vec4_div_mut(vec4* v1, vec4 v2);

float vec4_dot(vec4 v1, vec4 v2);

void vec4_normalize_mut(vec4* v);
vec4 vec4_get_normalized(vec4 v);

float vec4_dist(vec4 v1, vec4 v2);
float vec4_dist_sq(vec4 v1, vec4 v2);

vec4 vec4_lerp(vec4 v1, vec4 v2, float t);

// Conditional Functions
bool vec4_equals(vec4 v1, vec4 v2);

#endif
