#include "vec3.h"
#include <math.h>

#include "common_math.h"

vec3 vec3_construct(float s) {

    vec3 out;

    out.x = out.y = out.z = s;

    return out;
}
vec3 vec3_construct_sep(float x, float y, float z) {

    vec3 out;

    out.x = x;
    out.y = y;
    out.z = z;

    return out;
}

void vec3_construct_mut(vec3* v, float s) { v->x = v->y = v->z = s;}

void vec3_construct_sep_mut(vec3* v, float x, float y, float z) {

    v->x = x;
    v->y = y;
    v->z = z;
}

float vec3_magnitude(vec3 v1){ return sqrt((v1.x * v1.x) + (v1.y * v1.y) + (v1.z * v1.z)); }

vec3 vec3_zero() {
    vec3 out;
    vec3_construct_mut(&out, 0.0);

    return out;
}

vec3 vec3_add(vec3 v1, vec3 v2) {
    vec3 out;

    out.x = v1.x + v2.x;
    out.y = v1.y + v2.y;
    out.z = v1.z + v2.z;

    return out;
}


void vec3_add_mut(vec3* v1, vec3 v2) {

    v1->x += v2.x;
    v1->y += v2.y;
    v1->z += v2.z;

}


vec3 vec3_sub(vec3 v1, vec3 v2) {

    vec3 out;

    out.x = v1.x - v2.x;
    out.y = v1.y - v2.y;
    out.z = v1.z - v2.z;

    return out;
}


void vec3_sub_mut(vec3* v1, vec3 v2) {

    v1->x -= v2.x;
    v1->y -= v2.y;
    v1->z -= v2.z;
}


vec3 vec3_scale(vec3 v1, float s) {
    vec3 out;

    out.x = v1.x * s;
    out.y = v1.y * s;
    out.z = v1.z * s;

    return out;
}

void vec3_scale_mut(vec3* v1, float s) {

    v1->x *= s;
    v1->y *= s;
    v1->z *= s;
}

vec3 vec3_multiply(vec3 v1, vec3 v2) {
    vec3 out;

    out.x = v1.x * v2.x;
    out.y = v1.y * v2.y;
    out.z = v1.z * v2.z;

    return out;

}

void vec3_multiply_mut(vec3* v1, vec3 v2) {

    v1->x *= v2.x;
    v1->y *= v2.y;
    v1->z *= v2.z;

}


vec3 vec3_div(vec3 v1, vec3 v2) {
    // TODO: Low prio(?) error passing to handle 0 in vec3 element, or dont
    
    vec3 out;

    // todo: add equation function for floats to general math lib
    out.x = v2.x == 0.0 ? 0.0 : v1.x / v2.x;
    out.y = v2.y == 0.0 ? 0.0 : v1.y / v2.y;
    out.z = v2.z == 0.0 ? 0.0 : v1.z / v2.z;

    return out;
}

void vec3_div_mut(vec3* v1, vec3 v2) {

    v1->x = v2.x == 0.0 ? 0.0 : v1->x / v2.x;
    v1->y = v2.y == 0.0 ? 0.0 : v1->y / v2.y;
    v1->z = v2.z == 0.0 ? 0.0 : v1->z / v2.z;
}



float vec3_dot(vec3 v1, vec3 v2) {
    return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);

}



vec3 vec3_cross(vec3 v1, vec3 v2) {

    vec3 out;

    out.x = (v1.y * v2.z) + (v1.z * v2.y);
    out.y = (v1.x * v2.z) + (v1.z * v2.x);
    out.z = (v1.x * v2.y) + (v1.y * v2.x);

    return out;
}


void vec3_normalize_mut(vec3* v) {
    
    // TODO: Need proper float equals for this too
    
    float length = vec3_magnitude(*v);

    // v is already zero vector if its length is 0, but safety insurance set to 0
    if (length == 0.0) {
	vec3_construct_mut(v, 0.0); 
	return;
    }

    v->x /= length;
    v->y /= length;
    v->z /= length;
}

vec3 vec3_get_normalized(vec3 v) {
    // TODO: Need proper float equals for this too
   
    vec3 out;

    float length = vec3_magnitude(v);

    // v is already zero vector if its length is 0, but safety insurance set to 0
    if (length == 0.0) {
	vec3_construct_mut(&out, 0.0);
    }
    else {
    out.x = v.x / length;
    out.y = v.y / length;
    out.z = v.z / length;

    }

    return out;
}

float vec3_dist(vec3 v1, vec3 v2) {
    vec3 d = vec3_sub(v2, v1);

    return vec3_magnitude(d);
}

float vec3_dist_sq(vec3 v1, vec3 v2) {
    vec3 d = vec3_sub(v2, v1);

    return (d.x * d.x) + (d.y * d.y) + (d.z * d.z);
}

vec3 vec3_lerp(vec3 v1, vec3 v2, float t) {

    vec3 out;
    out.x = lerp_f(v1.x, v2.x, t);
    out.y = lerp_f(v1.y, v2.y, t);
    out.z = lerp_f(v1.z, v2.z, t);

    return out;
}

bool vec3_equals(vec3 v1, vec3 v2) {
    return v1.x == v2.x &&
	   v1.y == v2.y &&
	   v1.z == v2.z;
}
