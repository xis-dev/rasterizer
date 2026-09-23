#include "vec4.h"
#include <math.h>

#include "common_math.h"

vec4 vec4_construct(float s) {

    vec4 out;

    out.e[0] = out.e[1] = out.e[2] = out.e[3] = s;

    return out;
}
vec4 vec4_construct_sep(float x, float y, float z, float w) {

    vec4 out;

    out.e[0] = x;
    out.e[1] = y;
    out.e[2] = z;
    out.e[3] = w;

    return out;
}

void vec4_construct_mut(vec4* v, float s) { v->e[0] = v->e[1] = v->e[2] = v->e[3] = s;}

float vec4_magnitude(vec4 v1){ return sqrt((v1.e[0] * v1.e[0]) + (v1.e[1] * v1.e[1]) + (v1.e[2] * v1.e[2]) + (v1.e[3] * v1.e[3])); }

vec4 vec4_zero() {

    return vec4_construct(0.0);
}

vec4 vec4_add(vec4 v1, vec4 v2) {
    vec4_add_mut(&v1, v2);

    return v1;
}


void vec4_add_mut(vec4* v1, vec4 v2) {

    v1->e[0] += v2.e[0];
    v1->e[1] += v2.e[1];
    v1->e[2] += v2.e[2];
    v1->e[3] += v2.e[3];

}


vec4 vec4_sub(vec4 v1, vec4 v2) {

    vec4_sub_mut(&v1, v2);

    return v1;
}


void vec4_sub_mut(vec4* v1, vec4 v2) {

    v1->e[0] -= v2.e[0];
    v1->e[1] -= v2.e[1];
    v1->e[2] -= v2.e[2];
    v1->e[3] -= v2.e[3];

}


vec4 vec4_scale(vec4 v1, float s) {

    vec4_scale_mut(&v1, s);

    return v1;
}

void vec4_scale_mut(vec4* v1, float s) {

    v1->e[0] *= s;
    v1->e[1] *= s;
    v1->e[2] *= s;
    v1->e[3] *= s;
}

vec4 vec4_multiply(vec4 v1, vec4 v2) {

    vec4_multiply_mut(&v1, v2);

    return v1;
}

void vec4_multiply_mut(vec4* v1, vec4 v2) {

    v1->e[0] *= v2.e[0];
    v1->e[1] *= v2.e[1];
    v1->e[2] *= v2.e[2];
    v1->e[3] *= v2.e[3];

}


vec4 vec4_div(vec4 v1, vec4 v2) {

    vec4_div_mut(&v1, v2);

    return v1;
}

void vec4_div_mut(vec4* v1, vec4 v2) {

    // todo: add equation function for floats to general math lib
    v1->e[0] = v2.e[0] == 0.0 ? 0.0 : v1->e[0] / v2.e[0];
    v1->e[1] = v2.e[1] == 0.0 ? 0.0 : v1->e[1] / v2.e[1];
    v1->e[2] = v2.e[2] == 0.0 ? 0.0 : v1->e[2] / v2.e[2];
    v1->e[3] = v2.e[3] == 0.0 ? 0.0 : v1->e[3] / v2.e[3];
}



float vec4_dot(vec4 v1, vec4 v2) {
    return (v1.e[0] * v2.e[0]) + (v1.e[1] * v2.e[1]) + (v1.e[2] * v2.e[2]) + (v1.e[3] * v2.e[3]);

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
    v->e[0] *= one_over_length;
    v->e[1] *= one_over_length;
    v->e[2] *= one_over_length;
    v->e[3] *= one_over_length;

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

    return (d.e[0] * d.e[0]) + (d.e[1] * d.e[1]) + (d.e[2] * d.e[2]) + (d.e[3] * d.e[3]);
}

vec4 vec4_lerp(vec4 v1, vec4 v2, float t) {

    vec4 out;
    out.e[0] = lerp_f(v1.e[0], v2.e[0], t);
    out.e[1] = lerp_f(v1.e[1], v2.e[1], t);
    out.e[2] = lerp_f(v1.e[2], v2.e[2], t);
    out.e[3] = lerp_f(v1.e[3], v2.e[3], t);

    return out;
}


bool vec4_equals(vec4 v1, vec4 v2) {
    return  v1.e[0] == v2.e[0] &&
            v1.e[1] == v2.e[1] &&
	        v1.e[2] == v2.e[2] &&
	        v1.e[3] == v2.e[3];

}
