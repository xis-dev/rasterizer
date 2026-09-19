#include "include/math/vec2.h"
#include <math.h>

#include "common_math.h"

vec2 vec2_construct(float s) {

    vec2 out;

    out.x = out.y = s;

    return out;
}

vec2 vec2_construct_sep(float x, float y) {

    vec2 out;

    out.x = x;
    out.y = y;

    return out;
}

void vec2_construct_mut(vec2* v, float s) { v->x = v->y = s;}

void vec2_construct_sep_mut(vec2* v, float x, float y) {

    v->x = x;
    v->y = y;
}


float vec2_magnitude(vec2 v1){ return sqrt((v1.x * v1.x) + (v1.y * v1.y)); }

vec2 vec2_zero() {
    vec2 out;
    vec2_construct_mut(&out, 0.0);

    return out;
}

vec2 vec2_add(vec2 v1, vec2 v2) {
    vec2 out;

    out.x = v1.x + v2.x;
    out.y = v1.y + v2.y;

    return out;
}


void vec2_add_mut(vec2* v1, vec2 v2) {

    v1->x += v2.x;
    v1->y += v2.y;

}


vec2 vec2_sub(vec2 v1, vec2 v2) {

    vec2 out;

    out.x = v1.x - v2.x;
    out.y = v1.y - v2.y;

    return out;
}


void vec2_sub_mut(vec2* v1, vec2 v2) {

    v1->x -= v2.x;
    v1->y -= v2.y;
}


vec2 vec2_scale(vec2 v1, float s) {
    vec2 out;

    out.x = v1.x * s;
    out.y = v1.y * s;

    return out;
}

void vec2_scale_mut(vec2* v1, float s) {

    v1->x *= s;
    v1->y *= s;
}

vec2 vec2_multiply(vec2 v1, vec2 v2) {
    vec2 out;

    out.x = v1.x * v2.x;
    out.y = v1.y * v2.y;

    return out;

}

void vec2_multiply_mut(vec2* v1, vec2 v2) {

    v1->x *= v2.x;
    v1->y *= v2.y;

}

vec2 vec2_div(vec2 v1, vec2 v2) {
    // TODO: Low prio(?) error passing to handle 0 in vec2 element, or dont
    
    vec2 out;

    // todo: add equation function for floats to general math lib
    out.x = v2.x == 0.0 ? 0.0 : v1.x / v2.x;
    out.y = v2.y == 0.0 ? 0.0 : v1.y / v2.y;

    return out;
}

void vec2_div_mut(vec2* v1, vec2 v2) {

    v1->x = v2.x == 0.0 ? 0.0 : v1->x / v2.x;
    v1->y = v2.y == 0.0 ? 0.0 : v1->y / v2.y;
}

float vec2_dot(vec2 v1, vec2 v2) {
    return (v1.x * v2.x) + (v1.y * v2.y);

}


void vec2_normalize_mut(vec2* v) {
    
    // TODO: Need proper float equals for this too
    
    float length = vec2_magnitude(*v);

    // v is already zero vector if its length is 0, but safety insurance set to 0
    if (length == 0.0) {
	vec2_construct_mut(v, 0.0); 
	return;
    }

    v->x /= length;
    v->y /= length;
}

vec2 vec2_get_normalized(vec2 v) {
    // TODO: Need proper float equals for this too
   
    vec2 out;

    float length = vec2_magnitude(v);

    // v is already zero vector if its length is 0, but safety insurance set to 0
    if (length == 0.0) {
	vec2_construct_mut(&out, 0.0);
    }
    else {
    out.x = v.x / length;
    out.y = v.y / length;

    }

    return out;
}

float vec2_dist(vec2 v1, vec2 v2) {
    vec2 d = vec2_sub(v2, v1);

    return vec2_magnitude(d);
}

float vec2_dist_sq(vec2 v1, vec2 v2) {
    vec2 d = vec2_sub(v1, v2);

    return (d.x * d.x) + (d.y * d.y);
}

vec2 vec2_lerp(vec2 v1, vec2 v2, float t) {

    vec2 out;
    out.x = lerp_f(v1.x, v2.x, t);
    out.y = lerp_f(v1.y, v2.y, t);

    return out;
}

bool vec2_equals(vec2 v1, vec2 v2) {
    return v1.x == v2.x &&
	   v1.y == v2.y;
}
