#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <threads.h>

#include "../include/math/vec3.h"

#include "renderer.h"
#include "texture.h"

#define WIDTH 1600
#define HEIGHT 900


int main(int argc, char* argv[]) {

    mat4 model = mat4_construct_diagonal(1.0f);
    //model = mat4_scale_uniform(model, 5.0f);
    //model = mat4_rotate_by_axis(model, 45.0f, (vec3){1.0f, 0.0f, 0.0f});
    model = mat4_translate(model, (vec3){0.f, 0.0f, 10.2f});
    mat4 view = mat4_lookat((vec3){0.0f, 0.0f, 0.0f}, (vec3){0.0f, 0.0f, 1.0f});

    mat4 projection = mat4_projection(0.1f, 1000.0f, 45.0f, 16.0f/9.0f);

    context ctx;
    context_default_initialize(&ctx);

    sv_create(model, mat4,      &ctx.uniform_buffer[UNIFORM_SLOT_1]);
    sv_create(view, mat4,       &ctx.uniform_buffer[UNIFORM_SLOT_2]);
    sv_create(projection, mat4, &ctx.uniform_buffer[UNIFORM_SLOT_3]);

    sv_create(mat4_mult_multiple(2, view, model), mat4, &ctx.uniform_buffer[UNIFORM_SLOT_4]);
    sv_create(mat4_mult_multiple(3, projection, view, model), mat4, &ctx.uniform_buffer[UNIFORM_SLOT_5]);

    sv_create(((vec3){0.0f, 0.0f, 0.0f}), vec3, &ctx.uniform_buffer[UNIFORM_SLOT_6]);

    context_set_viewport_size(&ctx, 1600, 900);

    vertex vertices[4] = {
        (vertex){.pos = (vec4){-1.0f, -1.0f, 0.0f, 1.0f}, .normal = (vec3){0.0f, 0.0f, 1.0f}, .uv_0 = (vec2){0.0f, 0.0f}},
        (vertex){.pos = (vec4){-1.0f,  1.0f, 0.0f, 1.0f}, .normal = (vec3){0.0f, 0.0f, 1.0f}, .uv_0 = (vec2){0.0f, 1.0f}},
        (vertex){.pos = (vec4){ 1.0f,  1.0f, 0.0f, 1.0f}, .normal = (vec3){0.0f, 0.0f, 1.0f}, .uv_0 = (vec2){1.0f, 1.0f}},
        (vertex){.pos = (vec4){ 1.0f, -1.0f, 0.0f, 1.0f}, .normal = (vec3){0.0f, 0.0f, 1.0f}, .uv_0 = (vec2){1.0f, 0.0f}}
    };


    vertex triangle_vertices[4] = {
        (vertex){.pos = (vec4){-1.0f, -1.0f, 0.0f, 1.0f}, .colour = (vec3){1.0f,0.0f, 0.0f}, .normal = (vec3){0.0f, 0.0f, 1.0f}, .uv_0 = (vec2){0.0f, 0.0f}},
        (vertex){.pos = (vec4){ 0.0f,  1.0f, 0.0f, 1.0f}, .colour = (vec3){0.0f, 1.0f, 0.0f}, .normal = (vec3){0.0f, 0.0f, 1.0f}, .uv_0 = (vec2){0.0f, 1.0f}},
        (vertex){.pos = (vec4){ 1.0f,  -1.0f, 0.0f, 1.0f}, .colour = (vec3){0.0f, 0.0f, 1.0f}, .normal = (vec3){0.0f, 0.0f, 1.0f}, .uv_0 = (vec2){1.0f, 1.0f}},
    };

    uint32_t indices[6] = {
        0, 1, 3,
        3, 1, 2
    };

    texture house_tex;
    texture_load_texture(&house_tex, "../doctor-house.png", true);

    framebuffer basic;
    framebuffer_construct(&basic, 1600, 900);

    ctx.out_buffer = &basic;

    ctx.vertex_buffer = triangle_vertices;
    ctx.index_buffer = indices;

    ctx.shader = (shader_program){.vertex_shader = &default_vert_shader, .fragment_shader = &default_triangle_frag};

    ctx.textures[TEXTURE_SLOT_1] = &house_tex;

    point_light light = {0};
    light.intensity = 1.0f;
    p_light_atten_from_radius(&light, 50.0f);

    ctx.point_lights[0] = light;

    material random_mat = {.color = (colour3){1.0f, 1.0f, 0.0f},  .diffuse = 1.0f};

    ctx.material = random_mat;

    context_clear_colour(&ctx, (vec4){0.0f, 0.0f, 0.0f});
    context_clear_depth(&ctx);

//    draw_indexed_triangles(&ctx, 6);

    draw_listed_triangles(&ctx, 3);
    context_output_image_ppm(&ctx, "../image.ppm");

    context_cleanup(&ctx);

    framebuffer_destroy(&basic);

    return 0;
}
