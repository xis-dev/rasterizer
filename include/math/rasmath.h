#ifndef RASMATH_H
#define RASMATH_H

#include "vec2.h"
#include "vec3.h"
#include "vec4.h"
#include "mat4.h"

/*
 * x_convert_y functions define a conversion returning a value of type x from a value of type y
 */

vec3 vec3_convert_vec2(vec2 v, float z);
vec4 vec4_convert_vec2(vec2 v, float z, float w);

vec2 vec2_convert_vec3(vec3 v);
vec4 vec4_convert_vec3(vec3 v, float w);

vec2 vec2_convert_vec4(vec4 v);
vec3 vec3_convert_vec4(vec4 v);


#endif