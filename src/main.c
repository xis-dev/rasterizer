#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <threads.h>

#include "../include/math/vec3.h"

#include "renderer.h"

#define WIDTH 1600
#define HEIGHT 900


int main(int argc, char* argv[]) {

    mat4 model = mat4_construct_diagonal(1.0f);
    model = mat4_rotate_by_axis(model, 45.0f, (vec3){1.0f, 0.0f, 0.0f});
    model = mat4_translate(model, (vec3){5.f, 0.0f, 10.2f});

    mat4 view = mat4_lookat((vec3){0.0f, 0.0f, 0.0f}, (vec3){0.0f, 0.0f, 1.0f});

    mat4 projection = mat4_projection(0.1f, 1000.0f, 45.0f, 16.0f/9.0f);

    shader_value sv_model;
    shader_value sv_view;
    shader_value sv_projection;
    shader_value sv_model_view;
    shader_value sv_model_view_projection;

    sv_create(model, mat4, &sv_model);
    sv_create(view, mat4, &sv_view);
    sv_create(projection, mat4, &sv_projection);

    sv_create(mat4_mult_multiple(2, view, model), mat4, &sv_model_view);
    sv_create(mat4_mult_multiple(3, projection, view, model), mat4, &sv_model_view_projection);


    shader_value uniforms[] = {
        [UNIFORM_SLOT_1] = sv_model,
        [UNIFORM_SLOT_2] = sv_view,
        [UNIFORM_SLOT_3] = sv_projection,
        [UNIFORM_SLOT_4] = sv_model_view,
        [UNIFORM_SLOT_5] = sv_model_view_projection
    };

    //sv_change_value(&uniforms[UNIFORM_SLOT_1], view);

    context ctx;
    context_default_initialize(&ctx);

    context_set_viewport_size(&ctx, 1600, 900);
    context_set_uniform_array(&ctx, uniforms, 5);

    vertex vertices[4] = {
        (vertex){.pos = (vec4){-1.0f, -1.0f, 0.0f, 1.0f}},
        (vertex){.pos = (vec4){-1.0f,  1.0f, 0.0f, 1.0f}},
        (vertex){.pos = (vec4){ 1.0f,  1.0f, 0.0f, 1.0f}},
        (vertex){.pos = (vec4){ 1.0f, -1.0f, 0.0f, 1.0f}}
    };

    uint32_t indices[6] = {
        0, 1, 3,
        3, 1, 2
    };

    ctx.vertex_buffer = vertices;
    ctx.index_buffer = indices;

    ctx.shader = (shader_program){.vertex_shader = &default_vert_shader, .fragment_shader = &default_frag_shader};

    framebuffer basic;
    framebuffer_construct(&basic, 1600, 900);

    ctx.out_buffer = &basic;

    context_clear_colour(&ctx, (vec4){0.0f, 0.0f, 0.0f});
    context_clear_depth(&ctx);

    draw_indexed_triangles(&ctx, 6);

    context_output_image_ppm(&ctx, "../image.ppm");

    context_cleanup(&ctx);

    for (size_t i = 0; i < 5; ++i) {
        sv_destroy(&uniforms[i]);
    }

    framebuffer_destroy(&basic);

    return 0;
}
