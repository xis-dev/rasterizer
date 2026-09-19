#ifndef VEC2_H
#define VEC2_H

#include "vec3.h"
#include <stdbool.h>

/**
 * Structure of elements for a floating point 3D Vector
 * trailing '_mut' signifies mutating the first vector passed in
 * '_sep' signifies a vector is required but the expected arguments are seperated into its individual x,y components
 * 'x_convert_y' signifies a function converting an object of type x to y
*/
typedef struct vec2 {

    float x;
    float y;

} vec2;


// Initialize vector with the same scalar value in all elements
vec2 vec2_construct(float s);
vec2 vec2_construct_sep(float x, float y);

void vec2_construct_mut(vec2* v, float s);
void vec2_construct_sep_mut(vec2* v, float x, float y);


float vec2_magnitude(vec2 v);

vec2 vec2_zero();

vec2 vec2_add(vec2 v1, vec2 v2);
void vec2_add_mut(vec2* v1, vec2 v2);

vec2 vec2_sub(vec2 v1, vec2 v2);
void vec2_sub_mut(vec2* v1, vec2 v2);

vec2 vec2_scale(vec2 v1, float s);
void vec2_scale_mut(vec2* v1, float s);

// mutl & div define multiplication and division for each element of a vector
vec2 vec2_multiply(vec2 v1, vec2 v2);
void vec2_multiply_mut(vec2* v1, vec2 v2);

vec2 vec2_div(vec2 v1, vec2 v2);
void vec2_div_mut(vec2* v1, vec2 v2);

float vec2_dot(vec2 v1, vec2 v2);

void vec2_normalize_mut(vec2* v);
vec2 vec2_get_normalized(vec2 v);

float vec2_dist(vec2 v1, vec2 v2);
float vec2_dist_sq(vec2 v1, vec2 v2);

vec2 vec2_lerp(vec2 v1, vec2 v2, float t);

// Conditional Functions
bool vec2_equals(vec2 v1, vec2 v2);

#endif
