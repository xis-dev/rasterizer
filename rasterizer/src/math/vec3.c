#include "vec3.h"
#include <math.h>

#include "common_math.h"

vec3 vec3_construct(float s) {

    vec3 out;

    out.e[0] = out.e[1] = out.e[2] = s;

    return out;
}
vec3 vec3_construct_sep(float x, float y, float z) {

    vec3 out;

    out.e[0] = x;
    out.e[1] = y;
    out.e[2] = z;

    return out;
}

void vec3_construct_mut(vec3* v, float s) { v->e[0] = v->e[1] = v->e[2] = s;}

void vec3_construct_sep_mut(vec3* v, float x, float y, float z) {

    v->e[0] = x;
    v->e[1] = y;
    v->e[2] = z;
}

float vec3_magnitude(vec3 v1){ return sqrt((v1.e[0] * v1.e[0]) + (v1.e[1] * v1.e[1]) + (v1.e[2] * v1.e[2])); }

vec3 vec3_zero() {
    vec3 out;
    vec3_construct_mut(&out, 0.0);

    return out;
}

vec3 vec3_add(vec3 v1, vec3 v2) {
    vec3 out;

    out.e[0] = v1.e[0] + v2.e[0];
    out.e[1] = v1.e[1] + v2.e[1];
    out.e[2] = v1.e[2] + v2.e[2];

    return out;
}


void vec3_add_mut(vec3* v1, vec3 v2) {

    v1->e[0] += v2.e[0];
    v1->e[1] += v2.e[1];
    v1->e[2] += v2.e[2];

}


vec3 vec3_sub(vec3 v1, vec3 v2) {

    vec3 out;

    out.e[0] = v1.e[0] - v2.e[0];
    out.e[1] = v1.e[1] - v2.e[1];
    out.e[2] = v1.e[2] - v2.e[2];

    return out;
}


void vec3_sub_mut(vec3* v1, vec3 v2) {

    v1->e[0] -= v2.e[0];
    v1->e[1] -= v2.e[1];
    v1->e[2] -= v2.e[2];
}


vec3 vec3_scale(vec3 v1, float s) {
    vec3 out;

    out.e[0] = v1.e[0] * s;
    out.e[1] = v1.e[1] * s;
    out.e[2] = v1.e[2] * s;

    return out;
}

void vec3_scale_mut(vec3* v1, float s) {

    v1->e[0] *= s;
    v1->e[1] *= s;
    v1->e[2] *= s;
}

vec3 vec3_multiply(vec3 v1, vec3 v2) {
    vec3 out;

    out.e[0] = v1.e[0] * v2.e[0];
    out.e[1] = v1.e[1] * v2.e[1];
    out.e[2] = v1.e[2] * v2.e[2];

    return out;

}

void vec3_multiply_mut(vec3* v1, vec3 v2) {

    v1->e[0] *= v2.e[0];
    v1->e[1] *= v2.e[1];
    v1->e[2] *= v2.e[2];

}


vec3 vec3_div(vec3 v1, vec3 v2) {
    // TODO: Low prio(?) error passing to handle 0 in vec3 element, or dont

    vec3 out;

    // todo: add equation function for floats to general math lib
    out.e[0] = v2.e[0] == 0.0 ? 0.0 : v1.e[0] / v2.e[0];
    out.e[1] = v2.e[1] == 0.0 ? 0.0 : v1.e[1] / v2.e[1];
    out.e[2] = v2.e[2] == 0.0 ? 0.0 : v1.e[2] / v2.e[2];

    return out;
}

void vec3_div_mut(vec3* v1, vec3 v2) {

    v1->e[0] = v2.e[0] == 0.0 ? 0.0 : v1->e[0] / v2.e[0];
    v1->e[1] = v2.e[1] == 0.0 ? 0.0 : v1->e[1] / v2.e[1];
    v1->e[2] = v2.e[2] == 0.0 ? 0.0 : v1->e[2] / v2.e[2];
}



float vec3_dot(vec3 v1, vec3 v2) {
    return (v1.e[0] * v2.e[0]) + (v1.e[1] * v2.e[1]) + (v1.e[2] * v2.e[2]);

}



vec3 vec3_cross(vec3 v1, vec3 v2) {

    vec3 out;

    out.e[0] = (v1.e[1] * v2.e[2]) - (v1.e[2] * v2.e[1]);
    out.e[1] = (v1.e[0] * v2.e[2]) - (v1.e[2] * v2.e[0]);
    out.e[2] = (v1.e[0] * v2.e[1]) - (v1.e[1] * v2.e[0]);

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

    v->e[0] /= length;
    v->e[1] /= length;
    v->e[2] /= length;
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
    out.e[0] = v.e[0] / length;
    out.e[1] = v.e[1] / length;
    out.e[2] = v.e[2] / length;

    }

    return out;
}

float vec3_dist(vec3 v1, vec3 v2) {
    vec3 d = vec3_sub(v2, v1);

    return vec3_magnitude(d);
}

float vec3_dist_sq(vec3 v1, vec3 v2) {
    vec3 d = vec3_sub(v2, v1);

    return (d.e[0] * d.e[0]) + (d.e[1] * d.e[1]) + (d.e[2] * d.e[2]);
}

vec3 vec3_lerp(vec3 v1, vec3 v2, float t) {

    vec3 out;
    out.e[0] = lerp_f(v1.e[0], v2.e[0], t);
    out.e[1] = lerp_f(v1.e[1], v2.e[1], t);
    out.e[2] = lerp_f(v1.e[2], v2.e[2], t);

    return out;
}

bool vec3_equals(vec3 v1, vec3 v2) {
    return v1.e[0] == v2.e[0] &&
	   v1.e[1] == v2.e[1] &&
	   v1.e[2] == v2.e[2];
}
