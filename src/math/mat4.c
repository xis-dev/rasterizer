#include "mat4.h"
#include <math.h>
#include <stdarg.h>
#include <stdio.h>

mat4 mat4_construct_fill(float s) {
    mat4 out;
    out.c0 = out.c1 = out.c2 = out.c3 = vec4_construct(s);
    return out;
}

mat4 mat4_construct_diagonal(float s) {
    mat4 out;

    out.c0 = (vec4){.x = s};
    out.c1 = (vec4){.y = s};
    out.c2 = (vec4){.z = s};
    out.c3 = (vec4){.w = s};

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

    m_out->c0 = (vec4){m.c0.x * temp.c0.x + m.c1.x * temp.c0.y +
		    m.c2.x * temp.c0.z + m.c3.x * temp.c0.w,

		    m.c0.y * temp.c0.x + m.c1.y * temp.c0.y +
		    m.c2.y * temp.c0.z + m.c3.y * temp.c0.w,

		    m.c0.z * temp.c0.x + m.c1.z * temp.c0.y +
		    m.c2.z * temp.c0.z + m.c3.z * temp.c0.w,

		    m.c0.w * temp.c0.x + m.c1.w * temp.c0.y +
		    m.c2.w * temp.c0.z + m.c3.w * temp.c0.w};

    m_out->c1 = (vec4){m.c0.x * temp.c1.x + m.c1.x * temp.c1.y +
		    m.c2.x * temp.c1.z + m.c3.x * temp.c1.w,

		    m.c0.y * temp.c1.x + m.c1.y * temp.c1.y +
		    m.c2.y * temp.c1.z + m.c3.y * temp.c1.w,

		    m.c0.z * temp.c1.x + m.c1.z * temp.c1.y +
		    m.c2.z * temp.c1.z + m.c3.z * temp.c1.w,

		    m.c0.w * temp.c1.x + m.c1.w * temp.c1.y +
		    m.c2.w * temp.c1.z + m.c3.w * temp.c1.w};

    m_out->c2 = (vec4){m.c0.x * temp.c2.x + m.c1.x * temp.c2.y +
		    m.c2.x * temp.c2.z + m.c3.x * temp.c2.w,

		    m.c0.y * temp.c2.x + m.c1.y * temp.c2.y +
		    m.c2.y * temp.c2.z + m.c3.y * temp.c2.w,

		    m.c0.z * temp.c2.x + m.c1.z * temp.c2.y +
		    m.c2.z * temp.c2.z + m.c3.z * temp.c2.w,

		    m.c0.w * temp.c2.x + m.c1.w * temp.c2.y +
		    m.c2.w * temp.c2.z + m.c3.w * temp.c2.w};

    m_out->c3 = (vec4){m.c0.x * temp.c3.x + m.c1.x * temp.c3.y +
		    m.c2.x * temp.c3.z + m.c3.x * temp.c3.w,

		    m.c0.y * temp.c3.x + m.c1.y * temp.c3.y +
		    m.c2.y * temp.c3.z + m.c3.y * temp.c3.w,

		    m.c0.z * temp.c3.x + m.c1.z * temp.c3.y +
		    m.c2.z * temp.c3.z + m.c3.z * temp.c3.w,

		    m.c0.w * temp.c3.x + m.c1.w * temp.c3.y +
		    m.c2.w * temp.c3.z + m.c3.w * temp.c3.w};
}

mat4 mat4_get_transpose(mat4 m) {
    mat4_transpose(&m);
    return m;

}

void mat4_transpose(mat4* m) {

    mat4 temp = *m;

    m->c0 = (vec4){temp.c0.x, temp.c1.x, temp.c2.x, temp.c3.x};
    m->c1 = (vec4){temp.c0.y, temp.c1.y, temp.c2.y, temp.c3.y};
    m->c2 = (vec4){temp.c0.z, temp.c1.z, temp.c2.z, temp.c3.z};
    m->c3 = (vec4){temp.c0.w, temp.c1.w, temp.c2.w, temp.c3.w};
}

mat4 mat4_rotate_by_axis(mat4 m, double angle, vec3 axis) {

    mat4 out;

    out.c0 = (vec4){(axis.x * axis.x) * (1 - cos(angle)) + cos(angle),
		    (axis.x * axis.y) * (1 - cos(angle)) + axis.z * sin(angle),
		    (axis.x * axis.z) * (1 - cos(angle)) - axis.y * sin(angle), 0.0};

    out.c1 = (vec4){(axis.x * axis.y) * (1 - cos(angle)) - axis.z * sin(angle),
		    (axis.y * axis.y) * (1 - cos(angle)) + cos(angle),
		    (axis.y * axis.z) * (1 - cos(angle)) + axis.x * sin(angle), 0.0};

    out.c2 = (vec4){(axis.x * axis.z) * (1 - cos(angle)) + axis.y * sin(angle),
		    (axis.y * axis.z) * (1 - cos(angle)) - axis.x * sin(angle),
		    (axis.z * axis.z) * (1 - cos(angle)) + cos(angle), 0.0};

    out.c3 = (vec4){0.0, 0.0, 0.0, 1.0};

    return out;
}
mat4 mat4_scale_by_axis(mat4 m, double s, vec3 axis) {

    mat4 out;

    out.c0 = (vec4){1 + (s - 1) * (axis.x * axis.x),
			(s - 1) * (axis.x * axis.y),
			(s - 1) * (axis.x * axis.z), 0.0};

    out.c1 = (vec4){	(s - 1) * (axis.y * axis.x),
		    1 + (s - 1) * (axis.y * axis.y),
			(s - 1) * (axis.y * axis.z), 0.0};

    out.c2 = (vec4){	(s - 1) * (axis.z * axis.x),
			(s - 1) * (axis.z * axis.y),
		    1 + (s - 1) * (axis.z * axis.z), 0.0};

    out.c3 = (vec4){0.0, 0.0, 0.0, 1.0};

    return out;
}

mat4 mat4_translate(mat4 m, vec3 translation) {

    m.c3.x += translation.x;
    m.c3.y += translation.y;
    m.c3.z += translation.z;

    return m;
}

mat4 mat4_scale_uniform(mat4 m, double s) {

    return mat4_scale(m, vec3_construct(s));
}

mat4 mat4_scale(mat4 m, vec3 scale) {

    m.c0.x *= scale.x;
    m.c1.y *= scale.y;
    m.c2.z *= scale.z;

    return m;
}

vec4 mat4_transform(mat4 m, vec4 v) {

	vec4 out;
	out.x = v.x * m.c0.x + v.y * m.c1.x + v.z * m.c2.x + v.w * m.c3.x;
	out.y = v.x * m.c0.y + v.y * m.c1.y + v.z * m.c2.y + v.w * m.c3.y;
	out.z = v.x * m.c0.z + v.y * m.c1.z + v.z * m.c2.z + v.w * m.c3.z;
	out.w = v.x * m.c0.w + v.y * m.c1.w + v.z * m.c2.w + v.w * m.c3.w;

	return out;

}

mat4 mat4_lookat(vec3 eye, vec3 target) {

	mat4 out;

	vec3 cam_forward = vec3_sub(eye, target);

	vec3 g_up = {.x = 0.0f, .y = 1.0f, .z = 0.0f};

	// Generic up and camera forward are near parallel, use different vector
	if (fabsf(vec3_dot(g_up, cam_forward)) >= 0.99999f) {
		g_up = (vec3){.x = 0.0f, .y = 0.0f, .z = 1.0f};
	}

	// Take only portion of g_up perpendicular to cam_forward
	vec3 cam_up = vec3_sub(g_up, vec3_scale(cam_forward, vec3_dot(g_up, cam_forward)));

	vec3 cam_right = vec3_cross(cam_up, cam_forward);

	out.c0 = (vec4){cam_right.x, cam_up.x, cam_forward.x, 0.0f};
	out.c1 = (vec4){cam_right.y, cam_up.y, cam_forward.y, 0.0f};
	out.c2 = (vec4){cam_right.z, cam_up.z, cam_forward.z, 0.0f};
	out.c3 = (vec4){-vec3_dot(cam_right, eye), -vec3_dot(cam_up,eye), -vec3_dot(cam_forward, eye), 1.0f};

	return out;
}

// Projection matrix mapping -w <= x,y <= w and 0 <= z <= w
mat4 mat4_projection(float near, float far, float fovy, float aspect) {

	float zoom_y = 1.0f / tanf(fovy / 2.0f);
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
