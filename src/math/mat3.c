#include "mat3.h"
#include <math.h>
#include <stdarg.h>
#include <stdio.h>

#include "common_math.h"

mat3 mat3_construct_fill(float s) {
    mat3 out;
    out.c0 = out.c1 = out.c2 = vec3_construct(s);
    return out;
}

mat3 mat3_construct_diagonal(float s) {
    mat3 out;

    out.c0 = (vec3){.x = s};
    out.c1 = (vec3){.y = s};
    out.c2 = (vec3){.z = s};

    return out;
}

mat3 mat3_construct(vec3 _c0, vec3 _c1, vec3 _c2) {
    mat3 out = {.c0 = _c0, .c1 = _c1, .c2 = _c2};

    return out;
}

mat3 mat3_add(mat3 m0, mat3 m1) {
    mat3 out;
    out.c0 = vec3_add(m0.c0, m1.c0);
    out.c1 = vec3_add(m0.c1, m1.c1);
    out.c2 = vec3_add(m0.c2, m1.c2);

    return out;
}

mat3 mat3_add_multiple(size_t count, ...) {

    va_list matrices;

    va_start(matrices, count);

    mat3 sum = va_arg(matrices, mat3);

    // Loop from second arg until the second to last argument
    for (int i = 1; i < count; ++i) {
        mat3_add_mut(&sum, va_arg(matrices, mat3));
    }

    va_end(matrices);

    return sum;
}

void mat3_add_mut(mat3* m0, mat3 m1) {
    vec3_add_mut(&m0->c0, m1.c0);
    vec3_add_mut(&m0->c1, m1.c1);
    vec3_add_mut(&m0->c2, m1.c2);

}
void mat3_add_multiple_mut(mat3* m0, size_t count, ...) {

    va_list matrices;

    va_start(matrices, count);

    for (int i = 0; i < count; ++i) {
	mat3_add_mut(m0, va_arg(matrices, mat3));
    }

    va_end(matrices);
}

mat3 mat3_sub(mat3 m0, mat3 m1) {
    mat3 out;
    out.c0 = vec3_sub(m0.c0, m1.c0);
    out.c1 = vec3_sub(m0.c1, m1.c1);
    out.c2 = vec3_sub(m0.c2, m1.c2);

    return out;
}

mat3 mat3_sub_multiple(size_t count, ...) {

    va_list matrices;

    va_start(matrices, count);

    mat3 sum = va_arg(matrices, mat3);

    // Loop from second arg until the second to last argument
    for (int i = 1; i < count; ++i) {
	mat3_sub_mut(&sum, va_arg(matrices, mat3));
    }

    va_end(matrices);

    return sum;
}

void mat3_sub_mut(mat3* m0, mat3 m1) {
    vec3_sub_mut(&m0->c0, m1.c0);
    vec3_sub_mut(&m0->c1, m1.c1);
    vec3_sub_mut(&m0->c2, m1.c2);

}
void mat3_sub_multiple_mut(mat3* m0, size_t count, ...) {

    va_list matrices;

    va_start(matrices, count);

    for (int i = 0; i < count; ++i) {
	mat3_sub_mut(m0, va_arg(matrices, mat3));
    }

    va_end(matrices);
}

mat3 mat3_mult(mat3 m0, mat3 m1) {

    mat3_mult_mut(m0, &m1);

    return m1;
}
mat3 mat3_mult_multiple(size_t count, ...) {

    va_list matrices;

    va_start(matrices, count);

    mat3 mat_arr[count];

    // Store all matrices to be multiplied
    for (int i = 0; i < count; ++i) {
	mat_arr[i] = va_arg(matrices, mat3);
    }

    // Start from end of array and multiply
    mat3 out = mat_arr[count - 1];

    for (int i = count - 2; i >= 0; --i) {
	mat3_mult_mut(mat_arr[i], &out);
    }

    va_end(matrices);

    return out;
}

void mat3_mult_mut(mat3 m, mat3* m_out) {

    mat3 temp= *m_out;

    m_out->c0 = (vec3){m.c0.x * temp.c0.x + m.c1.x * temp.c0.y +
		    m.c2.x * temp.c0.z,

		    m.c0.y * temp.c0.x + m.c1.y * temp.c0.y +
		    m.c2.y * temp.c0.z,

		    m.c0.z * temp.c0.x + m.c1.z * temp.c0.y +
		    m.c2.z * temp.c0.z};

    m_out->c1 = (vec3){m.c0.x * temp.c1.x + m.c1.x * temp.c1.y +
		    m.c2.x * temp.c1.z,

		    m.c0.y * temp.c1.x + m.c1.y * temp.c1.y +
		    m.c2.y * temp.c1.z,

		    m.c0.z * temp.c1.x + m.c1.z * temp.c1.y +
		    m.c2.z * temp.c1.z};

    m_out->c2 = (vec3){m.c0.x * temp.c2.x + m.c1.x * temp.c2.y +
		    m.c2.x * temp.c2.z,

		    m.c0.y * temp.c2.x + m.c1.y * temp.c2.y +
		    m.c2.y * temp.c2.z,

		    m.c0.z * temp.c2.x + m.c1.z * temp.c2.y +
		    m.c2.z * temp.c2.z};

}

mat3 mat3_get_transpose(mat3 m) {
    mat3_transpose(&m);
    return m;

}

void mat3_transpose(mat3* m) {

    mat3 temp = *m;

    m->c0 = (vec3){temp.c0.x, temp.c1.x, temp.c2.x};
    m->c1 = (vec3){temp.c0.y, temp.c1.y, temp.c2.y};
    m->c2 = (vec3){temp.c0.z, temp.c1.z, temp.c2.z};

}

mat3 mat3_rotate_by_axis(mat3 m, double angle_deg, vec3 axis) {

    mat3 out;

	double angle_rad = (angle_deg * CM_PI) / 180.0f;

    out.c0 = (vec3){(axis.x * axis.x) * (1 - cos(angle_rad)) + cos(angle_rad),
		    (axis.x * axis.y) * (1 - cos(angle_rad)) + axis.z * sin(angle_rad),
		    (axis.x * axis.z) * (1 - cos(angle_rad)) - axis.y * sin(angle_rad)};

    out.c1 = (vec3){(axis.x * axis.y) * (1 - cos(angle_rad)) - axis.z * sin(angle_rad),
		    (axis.y * axis.y) * (1 - cos(angle_rad)) + cos(angle_rad),
		    (axis.y * axis.z) * (1 - cos(angle_rad)) + axis.x * sin(angle_rad)};

    out.c2 = (vec3){(axis.x * axis.z) * (1 - cos(angle_rad)) + axis.y * sin(angle_rad),
		    (axis.y * axis.z) * (1 - cos(angle_rad)) - axis.x * sin(angle_rad),
		    (axis.z * axis.z) * (1 - cos(angle_rad)) + cos(angle_rad)};

    out = mat3_mult(out, m);

    return out;
}
mat3 mat3_scale_by_axis(mat3 m, double s, vec3 axis) {

    mat3 out;

    out.c0 = (vec3){1 + (s - 1) * (axis.x * axis.x),
			(s - 1) * (axis.x * axis.y),
			(s - 1) * (axis.x * axis.z)};

    out.c1 = (vec3){	(s - 1) * (axis.y * axis.x),
		    1 + (s - 1) * (axis.y * axis.y),
			(s - 1) * (axis.y * axis.z)};

    out.c2 = (vec3){	(s - 1) * (axis.z * axis.x),
			(s - 1) * (axis.z * axis.y),
		    1 + (s - 1) * (axis.z * axis.z)};

    out = mat3_mult(out, m);

    return out;
}

mat3 mat3_scale_uniform(mat3 m, double s) {

    return mat3_scale(m, vec3_construct(s));
}

mat3 mat3_scale(mat3 m, vec3 scale) {

    m.c0.x *= scale.x;
    m.c1.y *= scale.y;
    m.c2.z *= scale.z;

    return m;
}

vec3 mat3_transform(mat3 m, vec3 v) {

	vec3 out;
	out.x = v.x * m.c0.x + v.y * m.c1.x + v.z * m.c2.x;
	out.y = v.x * m.c0.y + v.y * m.c1.y + v.z * m.c2.y;
	out.z = v.x * m.c0.z + v.y * m.c1.z + v.z * m.c2.z;

	return out;

}

float mat3_determinant(mat3 m) {
    return  m.c0.x * (m.c1.y * m.c2.z - m.c2.y * m.c1.z) -
            m.c1.x * (m.c0.y * m.c2.z - m.c2.y * m.c0.z) +
            m.c2.x * (m.c0.y * m.c1.z - m.c1.y * m.c0.z);
}

void mat3_inverse(mat3* m) {
    mat3 temp = *m;

    float det = temp.c0.x * (temp.c1.y * temp.c2.z - temp.c2.y * temp.c1.z) -
                temp.c1.x * (temp.c0.y * temp.c2.z - temp.c2.y * temp.c0.z) +
                temp.c2.x * (temp.c0.y * temp.c1.z - temp.c1.y * temp.c0.z);

    float inv_det = 1.0f / det;

    m->c0 = (vec3){(temp.c1.y * temp.c2.z - temp.c2.y * temp.c1.z) * inv_det,
                (temp.c2.y * temp.c0.z - temp.c0.y * temp.c2.z) * inv_det,
                (temp.c0.y * temp.c1.z - temp.c1.y * temp.c0.z) * inv_det};

    m->c1 = (vec3){(temp.c1.z * temp.c2.x - temp.c2.z * temp.c1.x) * inv_det,
                (temp.c2.z * temp.c0.x - temp.c0.z * temp.c2.x) * inv_det,
                (temp.c0.z * temp.c1.x - temp.c1.z * temp.c0.x) * inv_det};

    m->c2 = (vec3){(temp.c1.x * temp.c2.y - temp.c2.x * temp.c1.y) * inv_det,
                (temp.c2.x * temp.c0.y - temp.c0.x * temp.c2.y) * inv_det,
                (temp.c0.x * temp.c1.y - temp.c1.x * temp.c0.y) * inv_det};
}

mat3 mat3_get_inverse(mat3 m) {
    mat3_inverse(&m);
    return m;
}

void mat3_print(mat3 m) {};
