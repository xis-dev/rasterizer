#ifndef VEC3_H
#define VEC3_H

#include <stdbool.h>

/**
 * Structure of elements for a floating point 3D Vector
 * trailing '_mut' signifies mutating the first vector passed in
 * '_sep' signifies a vector is required but the expected arguments are seperated into its individual x,y, z components
*/
typedef struct vec3 {

    float x;
    float y;
    float z;

} vec3;

typedef vec3 colour3;
typedef vec3 point3;

// Initialize vector with the same scalar value in all elements
vec3 vec3_construct(float s);
vec3 vec3_construct_sep(float x, float y, float z);
void vec3_construct_mut(vec3* v, float s);
void vec3_construct_sep_mut(vec3* v, float x, float y, float z);

float vec3_magnitude(vec3 v1);

vec3 vec3_zero();

vec3 vec3_add(vec3 v1, vec3 v2);
void vec3_add_mut(vec3* v1, vec3 v2);

vec3 vec3_sub(vec3 v1, vec3 v2);
void vec3_sub_mut(vec3* v1, vec3 v2);

vec3 vec3_scale(vec3 v1, float s);
void vec3_scale_mut(vec3* v1, float s);

// mutl & div define multiplication and division for each element of a vector
vec3 vec3_multiply(vec3 v1, vec3 v2);
void vec3_multiply_mut(vec3* v1, vec3 v2);

vec3 vec3_div(vec3 v1, vec3 v2);
void vec3_div_mut(vec3* v1, vec3 v2);

float vec3_dot(vec3 v1, vec3 v2);

vec3 vec3_cross(vec3 v1, vec3 v2);

void vec3_normalize_mut(vec3* v);
vec3 vec3_get_normalized(vec3 v);

float vec3_dist(vec3 v1, vec3 v2);
float vec3_dist_sq(vec3 v1, vec3 v2);

vec3 vec3_lerp(vec3 v1, vec3 v2, float t);

// Conditional Functions
bool vec3_equals(vec3 v1, vec3 v2);

#endif
