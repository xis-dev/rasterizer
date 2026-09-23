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

    out.c0 = (vec3){.e[0] = s};
    out.c1 = (vec3){.e[1] = s};
    out.c2 = (vec3){.e[2] = s};

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

    m_out->c0 = (vec3){m.c0.e[0] * temp.c0.e[0] + m.c1.e[0] * temp.c0.e[1] +
		    m.c2.e[0] * temp.c0.e[2],

		    m.c0.e[1] * temp.c0.e[0] + m.c1.e[1] * temp.c0.e[1] +
		    m.c2.e[1] * temp.c0.e[2],

		    m.c0.e[2] * temp.c0.e[0] + m.c1.e[2] * temp.c0.e[1] +
		    m.c2.e[2] * temp.c0.e[2]};

    m_out->c1 = (vec3){m.c0.e[0] * temp.c1.e[0] + m.c1.e[0] * temp.c1.e[1] +
		    m.c2.e[0] * temp.c1.e[2],

		    m.c0.e[1] * temp.c1.e[0] + m.c1.e[1] * temp.c1.e[1] +
		    m.c2.e[1] * temp.c1.e[2],

		    m.c0.e[2] * temp.c1.e[0] + m.c1.e[2] * temp.c1.e[1] +
		    m.c2.e[2] * temp.c1.e[2]};

    m_out->c2 = (vec3){m.c0.e[0] * temp.c2.e[0] + m.c1.e[0] * temp.c2.e[1] +
		    m.c2.e[0] * temp.c2.e[2],

		    m.c0.e[1] * temp.c2.e[0] + m.c1.e[1] * temp.c2.e[1] +
		    m.c2.e[1] * temp.c2.e[2],

		    m.c0.e[2] * temp.c2.e[0] + m.c1.e[2] * temp.c2.e[1] +
		    m.c2.e[2] * temp.c2.e[2]};

}

mat3 mat3_get_transpose(mat3 m) {
    mat3_transpose(&m);
    return m;

}

void mat3_transpose(mat3* m) {

    mat3 temp = *m;

    m->c0 = (vec3){temp.c0.e[0], temp.c1.e[0], temp.c2.e[0]};
    m->c1 = (vec3){temp.c0.e[1], temp.c1.e[1], temp.c2.e[1]};
    m->c2 = (vec3){temp.c0.e[2], temp.c1.e[2], temp.c2.e[2]};

}

mat3 mat3_rotate_by_axis(mat3 m, double angle_deg, vec3 axis) {

    mat3 out;

	double angle_rad = (angle_deg * CM_PI) / 180.0f;

    out.c0 = (vec3){(axis.e[0] * axis.e[0]) * (1 - cos(angle_rad)) + cos(angle_rad),
		    (axis.e[0] * axis.e[1]) * (1 - cos(angle_rad)) + axis.e[2] * sin(angle_rad),
		    (axis.e[0] * axis.e[2]) * (1 - cos(angle_rad)) - axis.e[1] * sin(angle_rad)};

    out.c1 = (vec3){(axis.e[0] * axis.e[1]) * (1 - cos(angle_rad)) - axis.e[2] * sin(angle_rad),
		    (axis.e[1] * axis.e[1]) * (1 - cos(angle_rad)) + cos(angle_rad),
		    (axis.e[1] * axis.e[2]) * (1 - cos(angle_rad)) + axis.e[0] * sin(angle_rad)};

    out.c2 = (vec3){(axis.e[0] * axis.e[2]) * (1 - cos(angle_rad)) + axis.e[1] * sin(angle_rad),
		    (axis.e[1] * axis.e[2]) * (1 - cos(angle_rad)) - axis.e[0] * sin(angle_rad),
		    (axis.e[2] * axis.e[2]) * (1 - cos(angle_rad)) + cos(angle_rad)};

    out = mat3_mult(out, m);

    return out;
}
mat3 mat3_scale_by_axis(mat3 m, double s, vec3 axis) {

    mat3 out;

    out.c0 = (vec3){1 + (s - 1) * (axis.e[0] * axis.e[0]),
			(s - 1) * (axis.e[0] * axis.e[1]),
			(s - 1) * (axis.e[0] * axis.e[2])};

    out.c1 = (vec3){	(s - 1) * (axis.e[1] * axis.e[0]),
		    1 + (s - 1) * (axis.e[1] * axis.e[1]),
			(s - 1) * (axis.e[1] * axis.e[2])};

    out.c2 = (vec3){	(s - 1) * (axis.e[2] * axis.e[0]),
			(s - 1) * (axis.e[2] * axis.e[1]),
		    1 + (s - 1) * (axis.e[2] * axis.e[2])};

    out = mat3_mult(out, m);

    return out;
}

mat3 mat3_scale_uniform(mat3 m, double s) {

    return mat3_scale(m, vec3_construct(s));
}

mat3 mat3_scale(mat3 m, vec3 scale) {

    m.c0.e[0] *= scale.e[0];
    m.c1.e[1] *= scale.e[1];
    m.c2.e[2] *= scale.e[2];

    return m;
}

vec3 mat3_transform(mat3 m, vec3 v) {

	vec3 out;
	out.e[0] = v.e[0] * m.c0.e[0] + v.e[1] * m.c1.e[0] + v.e[2] * m.c2.e[0];
	out.e[1] = v.e[0] * m.c0.e[1] + v.e[1] * m.c1.e[1] + v.e[2] * m.c2.e[1];
	out.e[2] = v.e[0] * m.c0.e[2] + v.e[1] * m.c1.e[2] + v.e[2] * m.c2.e[2];

	return out;

}

float mat3_determinant(mat3 m) {
    return  m.c0.e[0] * (m.c1.e[1] * m.c2.e[2] - m.c2.e[1] * m.c1.e[2]) -
            m.c1.e[0] * (m.c0.e[1] * m.c2.e[2] - m.c2.e[1] * m.c0.e[2]) +
            m.c2.e[0] * (m.c0.e[1] * m.c1.e[2] - m.c1.e[1] * m.c0.e[2]);
}

void mat3_inverse(mat3* m) {
    mat3 temp = *m;

    float det = temp.c0.e[0] * (temp.c1.e[1] * temp.c2.e[2] - temp.c2.e[1] * temp.c1.e[2]) -
                temp.c1.e[0] * (temp.c0.e[1] * temp.c2.e[2] - temp.c2.e[1] * temp.c0.e[2]) +
                temp.c2.e[0] * (temp.c0.e[1] * temp.c1.e[2] - temp.c1.e[1] * temp.c0.e[2]);

    float inv_det = 1.0f / det;

    m->c0 = (vec3){(temp.c1.e[1] * temp.c2.e[2] - temp.c2.e[1] * temp.c1.e[2]) * inv_det,
                (temp.c2.e[1] * temp.c0.e[2] - temp.c0.e[1] * temp.c2.e[2]) * inv_det,
                (temp.c0.e[1] * temp.c1.e[2] - temp.c1.e[1] * temp.c0.e[2]) * inv_det};

    m->c1 = (vec3){(temp.c1.e[2] * temp.c2.e[0] - temp.c2.e[2] * temp.c1.e[0]) * inv_det,
                (temp.c2.e[2] * temp.c0.e[0] - temp.c0.e[2] * temp.c2.e[0]) * inv_det,
                (temp.c0.e[2] * temp.c1.e[0] - temp.c1.e[2] * temp.c0.e[0]) * inv_det};

    m->c2 = (vec3){(temp.c1.e[0] * temp.c2.e[1] - temp.c2.e[0] * temp.c1.e[1]) * inv_det,
                (temp.c2.e[0] * temp.c0.e[1] - temp.c0.e[0] * temp.c2.e[1]) * inv_det,
                (temp.c0.e[0] * temp.c1.e[1] - temp.c1.e[0] * temp.c0.e[1]) * inv_det};
}

mat3 mat3_get_inverse(mat3 m) {
    mat3_inverse(&m);
    return m;
}

void mat3_print(mat3 m) {};
