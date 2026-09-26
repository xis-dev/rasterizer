#include "math/mat4.h"
#include <math.h>
#include <stdarg.h>
#include <stdio.h>

#include "math/common_math.h"

mat4 mat4_construct_fill(float s) {
    mat4 out;
    out.c0 = out.c1 = out.c2 = out.c3 = vec4_construct(s);
    return out;
}

mat4 mat4_construct_diagonal(float s) {
    mat4 out;

    out.c0 = (vec4){.e[0] = s};
    out.c1 = (vec4){.e[1] = s};
    out.c2 = (vec4){.e[2] = s};
    out.c3 = (vec4){.e[3] = s};

    return out;
}

mat4 mat4_construct(vec4 _c0, vec4 _c1, vec4 _c2, vec4 _c3) {
    mat4 out = {.c0 = _c0, .c1 = _c1, .c2 = _c2, .c3 = _c3};

    return out;
}

mat4 mat4_add(mat4 m0, mat4 m1) {
    mat4 out;
    out.c0 = vec4_add(m0.c0, m1.c0);
    out.c1 = vec4_add(m0.c1, m1.c1);
    out.c2 = vec4_add(m0.c2, m1.c2);
    out.c3 = vec4_add(m0.c3, m1.c3);

    return out;
}

mat4 mat4_add_multiple(size_t count, ...) {

    va_list matrices;

    va_start(matrices, count);

    mat4 sum = va_arg(matrices, mat4);

    // Loop from second arg until the second to last argument
    for (int i = 1; i < count; ++i) {
        mat4_add_mut(&sum, va_arg(matrices, mat4));
    }

    va_end(matrices);

    return sum;
}

void mat4_add_mut(mat4* m0, mat4 m1) {
    vec4_add_mut(&m0->c0, m1.c0);
    vec4_add_mut(&m0->c1, m1.c1);
    vec4_add_mut(&m0->c2, m1.c2);
    vec4_add_mut(&m0->c3, m1.c3);

}
void mat4_add_multiple_mut(mat4* m0, size_t count, ...) {

    va_list matrices;

    va_start(matrices, count);

    for (int i = 0; i < count; ++i) {
	mat4_add_mut(m0, va_arg(matrices, mat4));
    }

    va_end(matrices);
}

mat4 mat4_sub(mat4 m0, mat4 m1) {
    mat4 out;
    out.c0 = vec4_sub(m0.c0, m1.c0);
    out.c1 = vec4_sub(m0.c1, m1.c1);
    out.c2 = vec4_sub(m0.c2, m1.c2);
    out.c3 = vec4_sub(m0.c3, m1.c3);

    return out;
}

mat4 mat4_sub_multiple(size_t count, ...) {

    va_list matrices;

    va_start(matrices, count);

    mat4 sum = va_arg(matrices, mat4);

    // Loop from second arg until the second to last argument
    for (int i = 1; i < count; ++i) {
	mat4_sub_mut(&sum, va_arg(matrices, mat4));
    }

    va_end(matrices);

    return sum;
}

void mat4_sub_mut(mat4* m0, mat4 m1) {
    vec4_sub_mut(&m0->c0, m1.c0);
    vec4_sub_mut(&m0->c1, m1.c1);
    vec4_sub_mut(&m0->c2, m1.c2);
    vec4_sub_mut(&m0->c3, m1.c3);

}
void mat4_sub_multiple_mut(mat4* m0, size_t count, ...) {

    va_list matrices;

    va_start(matrices, count);

    for (int i = 0; i < count; ++i) {
	mat4_sub_mut(m0, va_arg(matrices, mat4));
    }

    va_end(matrices);
}

mat4 mat4_mult(mat4 m0, mat4 m1) {

    mat4_mult_mut(m0, &m1);

    return m1;
}
mat4 mat4_mult_multiple(size_t count, ...) {

    va_list matrices;

    va_start(matrices, count);

    mat4 mat_arr[count];

    // Store all matrices to be multiplied
    for (int i = 0; i < count; ++i) {
	mat_arr[i] = va_arg(matrices, mat4);
    }

    // Start from end of array and multiply
    mat4 out = mat_arr[count - 1];

    for (int i = count - 2; i >= 0; --i) {
	mat4_mult_mut(mat_arr[i], &out);
    }

    va_end(matrices);

    return out;
}

void mat4_mult_mut(mat4 m, mat4* m_out) {

    mat4 temp= *m_out;

    m_out->c0 = (vec4){m.c0.e[0] * temp.c0.e[0] + m.c1.e[0] * temp.c0.e[1] +
		    m.c2.e[0] * temp.c0.e[2] + m.c3.e[0] * temp.c0.e[3],

		    m.c0.e[1] * temp.c0.e[0] + m.c1.e[1] * temp.c0.e[1] +
		    m.c2.e[1] * temp.c0.e[2] + m.c3.e[1] * temp.c0.e[3],

		    m.c0.e[2] * temp.c0.e[0] + m.c1.e[2] * temp.c0.e[1] +
		    m.c2.e[2] * temp.c0.e[2] + m.c3.e[2] * temp.c0.e[3],

		    m.c0.e[3] * temp.c0.e[0] + m.c1.e[3] * temp.c0.e[1] +
		    m.c2.e[3] * temp.c0.e[2] + m.c3.e[3] * temp.c0.e[3]};

    m_out->c1 = (vec4){m.c0.e[0] * temp.c1.e[0] + m.c1.e[0] * temp.c1.e[1] +
		    m.c2.e[0] * temp.c1.e[2] + m.c3.e[0] * temp.c1.e[3],

		    m.c0.e[1] * temp.c1.e[0] + m.c1.e[1] * temp.c1.e[1] +
		    m.c2.e[1] * temp.c1.e[2] + m.c3.e[1] * temp.c1.e[3],

		    m.c0.e[2] * temp.c1.e[0] + m.c1.e[2] * temp.c1.e[1] +
		    m.c2.e[2] * temp.c1.e[2] + m.c3.e[2] * temp.c1.e[3],

		    m.c0.e[3] * temp.c1.e[0] + m.c1.e[3] * temp.c1.e[1] +
		    m.c2.e[3] * temp.c1.e[2] + m.c3.e[3] * temp.c1.e[3]};

    m_out->c2 = (vec4){m.c0.e[0] * temp.c2.e[0] + m.c1.e[0] * temp.c2.e[1] +
		    m.c2.e[0] * temp.c2.e[2] + m.c3.e[0] * temp.c2.e[3],

		    m.c0.e[1] * temp.c2.e[0] + m.c1.e[1] * temp.c2.e[1] +
		    m.c2.e[1] * temp.c2.e[2] + m.c3.e[1] * temp.c2.e[3],

		    m.c0.e[2] * temp.c2.e[0] + m.c1.e[2] * temp.c2.e[1] +
		    m.c2.e[2] * temp.c2.e[2] + m.c3.e[2] * temp.c2.e[3],

		    m.c0.e[3] * temp.c2.e[0] + m.c1.e[3] * temp.c2.e[1] +
		    m.c2.e[3] * temp.c2.e[2] + m.c3.e[3] * temp.c2.e[3]};

    m_out->c3 = (vec4){m.c0.e[0] * temp.c3.e[0] + m.c1.e[0] * temp.c3.e[1] +
		    m.c2.e[0] * temp.c3.e[2] + m.c3.e[0] * temp.c3.e[3],

		    m.c0.e[1] * temp.c3.e[0] + m.c1.e[1] * temp.c3.e[1] +
		    m.c2.e[1] * temp.c3.e[2] + m.c3.e[1] * temp.c3.e[3],

		    m.c0.e[2] * temp.c3.e[0] + m.c1.e[2] * temp.c3.e[1] +
		    m.c2.e[2] * temp.c3.e[2] + m.c3.e[2] * temp.c3.e[3],

		    m.c0.e[3] * temp.c3.e[0] + m.c1.e[3] * temp.c3.e[1] +
		    m.c2.e[3] * temp.c3.e[2] + m.c3.e[3] * temp.c3.e[3]};
}

mat4 mat4_get_transpose(mat4 m) {
    mat4_transpose(&m);
    return m;

}

void mat4_transpose(mat4* m) {

    mat4 temp = *m;

    m->c0 = (vec4){temp.c0.e[0], temp.c1.e[0], temp.c2.e[0], temp.c3.e[0]};
    m->c1 = (vec4){temp.c0.e[1], temp.c1.e[1], temp.c2.e[1], temp.c3.e[1]};
    m->c2 = (vec4){temp.c0.e[2], temp.c1.e[2], temp.c2.e[2], temp.c3.e[2]};
    m->c3 = (vec4){temp.c0.e[3], temp.c1.e[3], temp.c2.e[3], temp.c3.e[3]};
}

mat4 mat4_rotate_by_axis(mat4 m, double angle_deg, vec3 axis) {

    mat4 out;

	double angle_rad = (angle_deg * CM_PI) / 180.0f;

    out.c0 = (vec4){(axis.e[0] * axis.e[0]) * (1 - cos(angle_rad)) + cos(angle_rad),
		    (axis.e[0] * axis.e[1]) * (1 - cos(angle_rad)) + axis.e[2] * sin(angle_rad),
		    (axis.e[0] * axis.e[2]) * (1 - cos(angle_rad)) - axis.e[1] * sin(angle_rad), 0.0};

    out.c1 = (vec4){(axis.e[0] * axis.e[1]) * (1 - cos(angle_rad)) - axis.e[2] * sin(angle_rad),
		    (axis.e[1] * axis.e[1]) * (1 - cos(angle_rad)) + cos(angle_rad),
		    (axis.e[1] * axis.e[2]) * (1 - cos(angle_rad)) + axis.e[0] * sin(angle_rad), 0.0};

    out.c2 = (vec4){(axis.e[0] * axis.e[2]) * (1 - cos(angle_rad)) + axis.e[1] * sin(angle_rad),
		    (axis.e[1] * axis.e[2]) * (1 - cos(angle_rad)) - axis.e[0] * sin(angle_rad),
		    (axis.e[2] * axis.e[2]) * (1 - cos(angle_rad)) + cos(angle_rad), 0.0};

    out.c3 = (vec4){0.0, 0.0, 0.0, 1.0};

	out = mat4_mult(out, m);

    return out;
}
mat4 mat4_scale_by_axis(mat4 m, double s, vec3 axis) {

    mat4 out;

    out.c0 = (vec4){1 + (s - 1) * (axis.e[0] * axis.e[0]),
			(s - 1) * (axis.e[0] * axis.e[1]),
			(s - 1) * (axis.e[0] * axis.e[2]), 0.0};

    out.c1 = (vec4){	(s - 1) * (axis.e[1] * axis.e[0]),
		    1 + (s - 1) * (axis.e[1] * axis.e[1]),
			(s - 1) * (axis.e[1] * axis.e[2]), 0.0};

    out.c2 = (vec4){	(s - 1) * (axis.e[2] * axis.e[0]),
			(s - 1) * (axis.e[2] * axis.e[1]),
		    1 + (s - 1) * (axis.e[2] * axis.e[2]), 0.0};

    out.c3 = (vec4){0.0, 0.0, 0.0, 1.0};

	out = mat4_mult(out, m);

    return out;
}

mat4 mat4_translate(mat4 m, vec3 translation) {

    m.c3.e[0] += translation.e[0];
    m.c3.e[1] += translation.e[1];
    m.c3.e[2] += translation.e[2];

    return m;
}

mat4 mat4_scale_uniform(mat4 m, double s) {

    return mat4_scale(m, vec3_construct(s));
}

mat4 mat4_scale(mat4 m, vec3 scale) {

    m.c0.e[0] *= scale.e[0];
    m.c1.e[1] *= scale.e[1];
    m.c2.e[2] *= scale.e[2];

    return m;
}

vec4 mat4_transform(mat4 m, vec4 v) {

	vec4 out;
	out.e[0] = v.e[0] * m.c0.e[0] + v.e[1] * m.c1.e[0] + v.e[2] * m.c2.e[0] + v.e[3] * m.c3.e[0];
	out.e[1] = v.e[0] * m.c0.e[1] + v.e[1] * m.c1.e[1] + v.e[2] * m.c2.e[1] + v.e[3] * m.c3.e[1];
	out.e[2] = v.e[0] * m.c0.e[2] + v.e[1] * m.c1.e[2] + v.e[2] * m.c2.e[2] + v.e[3] * m.c3.e[2];
	out.e[3] = v.e[0] * m.c0.e[3] + v.e[1] * m.c1.e[3] + v.e[2] * m.c2.e[3] + v.e[3] * m.c3.e[3];

	return out;

}

mat4 mat4_lookat(vec3 eye, vec3 target) {

	mat4 out;

	vec3 cam_forward = vec3_get_normalized(vec3_sub(target, eye));

	vec3 g_up = {.e[0] = 0.0f, .e[1] = 1.0f, .e[2] = 0.0f};

	// Generic up and camera forward are near parallel, use different vector
	if (fabsf(vec3_dot(g_up, cam_forward)) >= 0.99999f) {
		g_up = (vec3){.e[0] = 0.0f, .e[1] = 0.0f, .e[2] = 1.0f};
	}

	// Take only portion of g_up perpendicular to cam_forward
	vec3 cam_up = vec3_get_normalized(vec3_sub(g_up, vec3_scale(cam_forward, vec3_dot(g_up, cam_forward))));

	vec3 cam_right = vec3_cross(cam_up, cam_forward);

	out.c0 = (vec4){cam_right.e[0], cam_up.e[0], cam_forward.e[0], 0.0f};
	out.c1 = (vec4){cam_right.e[1], cam_up.e[1], cam_forward.e[1], 0.0f};
	out.c2 = (vec4){cam_right.e[2], cam_up.e[2], cam_forward.e[2], 0.0f};
	out.c3 = (vec4){-vec3_dot(cam_right, eye), -vec3_dot(cam_up,eye), -vec3_dot(cam_forward, eye), 1.0f};

	return out;
}

// Projection matrix mapping -w <= x,y <= w and 0 <= z <= w
mat4 mat4_projection(float near, float far, float fovy, float aspect) {

	float zoom_y = 1.0f / tanf(((fovy / 2.0f) * (float)CM_PI ) / 180.0f);
	float zoom_x = zoom_y / aspect;

	float z_term = far / (far - near);
	float w_term = (near * far) / (near - far);

	mat4 out;
	out.c0 = (vec4){zoom_x, 0.0f, 0.0f, 0.0f};
	out.c1 = (vec4){0.0f, zoom_y, 0.0f, 0.0f};
	out.c2 = (vec4){0.0f, 0.0f, z_term, 1.0f};
	out.c3 = (vec4){0.0f, 0.0f, w_term, 0.0f};

	return out;
}



void mat4_print(mat4 m);
