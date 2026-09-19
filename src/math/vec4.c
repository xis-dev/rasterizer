#include "vec4.h"
#include <math.h>

#include "common_math.h"

vec4 vec4_construct(float s) {

    vec4 out;

    out.x = out.y = out.z = out.w = s;

    return out;
}
vec4 vec4_construct_sep(float x, float y, float z, float w) {

    vec4 out;

    out.x = x;
    out.y = y;
    out.z = z;
    out.w = w;

    return out;
}

void vec4_construct_mut(vec4* v, float s) { v->x = v->y = v->z = v->w = s;}

float vec4_magnitude(vec4 v1){ return sqrt((v1.x * v1.x) + (v1.y * v1.y) + (v1.z * v1.z) + (v1.w * v1.w)); }

vec4 vec4_zero() {

    return vec4_construct(0.0);
}

vec4 vec4_add(vec4 v1, vec4 v2) {
    vec4_add_mut(&v1, v2);

    return v1;
}


void vec4_add_mut(vec4* v1, vec4 v2) {

    v1->x += v2.x;
    v1->y += v2.y;
    v1->z += v2.z;
    v1->w += v2.w;

}


vec4 vec4_sub(vec4 v1, vec4 v2) {

    vec4_sub_mut(&v1, v2);

    return v1;
}


void vec4_sub_mut(vec4* v1, vec4 v2) {

    v1->x -= v2.x;
    v1->y -= v2.y;
    v1->z -= v2.z;
    v1->w -= v2.w;

}


vec4 vec4_scale(vec4 v1, float s) {

    vec4_scale_mut(&v1, s);

    return v1;
}

void vec4_scale_mut(vec4* v1, float s) {

    v1->x *= s;
    v1->y *= s;
    v1->z *= s;
    v1->w *= s;
}

vec4 vec4_multiply(vec4 v1, vec4 v2) {

    vec4_multiply_mut(&v1, v2);

    return v1;
}

void vec4_multiply_mut(vec4* v1, vec4 v2) {

    v1->x *= v2.x;
    v1->y *= v2.y;
    v1->z *= v2.z;
    v1->w *= v2.w;

}


vec4 vec4_div(vec4 v1, vec4 v2) {

    vec4_div_mut(&v1, v2);

    return v1;
}

void vec4_div_mut(vec4* v1, vec4 v2) {

    // todo: add equation function for floats to general math lib
    v1->x = v2.x == 0.0 ? 0.0 : v1->x / v2.x;
    v1->y = v2.y == 0.0 ? 0.0 : v1->y / v2.y;
    v1->z = v2.z == 0.0 ? 0.0 : v1->z / v2.z;
    v1->w = v2.w == 0.0 ? 0.0 : v1->w / v2.w;
}



float vec4_dot(vec4 v1, vec4 v2) {
    return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z) + (v1.w * v2.w);

}

void vec4_normalize_mut(vec4* v) {

    // TODO: Need proper float equals for this too
    float length = vec4_magnitude(*v);
    // v is already zero vector if its length is 0, but safety insurance set to 0
    if (length == 0.0) {
    	vec4_construct_mut(v, 0.0);
	    return;
    }

    float one_over_length = 1.0f  / length;
    v->x *= one_over_length;
    v->y *= one_over_length;
    v->z *= one_over_length;
    v->w *= one_over_length;

}

vec4 vec4_get_normalized(vec4 v) {

    vec4_normalize_mut(&v);

    return v;
}

float vec4_dist(vec4 v1, vec4 v2) {
    vec4 d = vec4_sub(v2, v1);

    return vec4_magnitude(d);
}

float vec4_dist_sq(vec4 v1, vec4 v2) {
    vec4 d = vec4_sub(v2, v1);

    return (d.x * d.x) + (d.y * d.y) + (d.z * d.z) + (d.w * d.w);
}

vec4 vec4_lerp(vec4 v1, vec4 v2, float t) {

    vec4 out;
    out.x = lerp_f(v1.x, v2.x, t);
    out.y = lerp_f(v1.y, v2.y, t);
    out.z = lerp_f(v1.z, v2.z, t);
    out.w = lerp_f(v1.w, v2.w, t);

    return out;
}


bool vec4_equals(vec4 v1, vec4 v2) {
    return  v1.x == v2.x &&
            v1.y == v2.y &&
	        v1.z == v2.z &&
	        v1.w == v2.w;

}

