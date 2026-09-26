#include "shaders.h"

#include "rasterizer/renderer.h"
#include "rasterizer/texture.h"
#include "rasterizer/framebuffer.h"
#include "rasterizer/light.h"
#include "rasterizer/material.h"
#include "rasterizer/shader_value.h"

#include <stdlib.h>

#define WIDTH 1600
#define HEIGHT 900


static vertex vertices[24] = {
    // Front Face
    (vertex){.pos = (vec4){-1.0f, -1.0f,  1.0f, 1.0f}, .normal = (vec3){ 0.0f,  0.0f,  1.0f}, .uv_0 = (vec2){0.0f, 0.0f}},
    (vertex){.pos = (vec4){-1.0f,  1.0f,  1.0f, 1.0f}, .normal = (vec3){ 0.0f,  0.0f,  1.0f}, .uv_0 = (vec2){0.0f, 1.0f}},
    (vertex){.pos = (vec4){ 1.0f,  1.0f,  1.0f, 1.0f}, .normal = (vec3){ 0.0f,  0.0f,  1.0f}, .uv_0 = (vec2){1.0f, 1.0f}},
    (vertex){.pos = (vec4){ 1.0f, -1.0f,  1.0f, 1.0f}, .normal = (vec3){ 0.0f,  0.0f,  1.0f}, .uv_0 = (vec2){1.0f, 0.0f}},

    // Back Face
    (vertex){.pos = (vec4){ 1.0f, -1.0f, -1.0f, 1.0f}, .normal = (vec3){ 0.0f,  0.0f, -1.0f}, .uv_0 = (vec2){0.0f, 0.0f}},
    (vertex){.pos = (vec4){ 1.0f,  1.0f, -1.0f, 1.0f}, .normal = (vec3){ 0.0f,  0.0f, -1.0f}, .uv_0 = (vec2){0.0f, 1.0f}},
    (vertex){.pos = (vec4){-1.0f,  1.0f, -1.0f, 1.0f}, .normal = (vec3){ 0.0f,  0.0f, -1.0f}, .uv_0 = (vec2){1.0f, 1.0f}},
    (vertex){.pos = (vec4){-1.0f, -1.0f, -1.0f, 1.0f}, .normal = (vec3){ 0.0f,  0.0f, -1.0f}, .uv_0 = (vec2){1.0f, 0.0f}},

    // Right Face
    (vertex){.pos = (vec4){ 1.0f, -1.0f,  1.0f, 1.0f}, .normal = (vec3){ 1.0f,  0.0f,  0.0f}, .uv_0 = (vec2){0.0f, 0.0f}},
    (vertex){.pos = (vec4){ 1.0f,  1.0f,  1.0f, 1.0f}, .normal = (vec3){ 1.0f,  0.0f,  0.0f}, .uv_0 = (vec2){0.0f, 1.0f}},
    (vertex){.pos = (vec4){ 1.0f,  1.0f, -1.0f, 1.0f}, .normal = (vec3){ 1.0f,  0.0f,  0.0f}, .uv_0 = (vec2){1.0f, 1.0f}},
    (vertex){.pos = (vec4){ 1.0f, -1.0f, -1.0f, 1.0f}, .normal = (vec3){ 1.0f,  0.0f,  0.0f}, .uv_0 = (vec2){1.0f, 0.0f}},

    // Left Face
    (vertex){.pos = (vec4){-1.0f, -1.0f, -1.0f, 1.0f}, .normal = (vec3){-1.0f,  0.0f,  0.0f}, .uv_0 = (vec2){0.0f, 0.0f}},
    (vertex){.pos = (vec4){-1.0f,  1.0f, -1.0f, 1.0f}, .normal = (vec3){-1.0f,  0.0f, 0.0f}, .uv_0 = (vec2){0.0f, 1.0f}},
    (vertex){.pos = (vec4){-1.0f,  1.0f,  1.0f, 1.0f}, .normal = (vec3){-1.0f,  0.0f, 0.0f}, .uv_0 = (vec2){1.0f, 1.0f}},
    (vertex){.pos = (vec4){-1.0f, -1.0f,  1.0f, 1.0f}, .normal = (vec3){-1.0f,  0.0f, 0.0f}, .uv_0 = (vec2){1.0f, 0.0f}},

    // Top Face
    (vertex){.pos = (vec4){-1.0f,  1.0f,  1.0f, 1.0f}, .normal = (vec3){ 0.0f,  1.0f,  0.0f}, .uv_0 = (vec2){0.0f, 0.0f}},
    (vertex){.pos = (vec4){-1.0f,  1.0f, -1.0f, 1.0f}, .normal = (vec3){ 0.0f,  1.0f,  0.0f}, .uv_0 = (vec2){0.0f, 1.0f}},
    (vertex){.pos = (vec4){ 1.0f,  1.0f, -1.0f, 1.0f}, .normal = (vec3){ 0.0f,  1.0f,  0.0f}, .uv_0 = (vec2){1.0f, 1.0f}},
    (vertex){.pos = (vec4){ 1.0f,  1.0f,  1.0f, 1.0f}, .normal = (vec3){ 0.0f,  1.0f, 0.0f}, .uv_0 = (vec2){1.0f, 0.0f}},

    // Bottom Face
    (vertex){.pos = (vec4){-1.0f, -1.0f, -1.0f, 1.0f}, .normal = (vec3){ 0.0f, -1.0f,  0.0f}, .uv_0 = (vec2){0.0f, 0.0f}},
    (vertex){.pos = (vec4){-1.0f, -1.0f,  1.0f, 1.0f}, .normal = (vec3){ 0.0f, -1.0f,  0.0f}, .uv_0 = (vec2){0.0f, 1.0f}},
    (vertex){.pos = (vec4){ 1.0f, -1.0f,  1.0f, 1.0f}, .normal = (vec3){ 0.0f, -1.0f,  0.0f}, .uv_0 = (vec2){1.0f, 1.0f}},
    (vertex){.pos = (vec4){ 1.0f, -1.0f, -1.0f, 1.0f}, .normal = (vec3){ 0.0f, -1.0f,  0.0f}, .uv_0 = (vec2){1.0f, 0.0f}}
};

static uint32_t indices[36] = {
    // Front Face
    0, 1, 2,   0, 2, 3,
    // Back Face
    4, 5, 6,   4, 6, 7,
    // Right Face
    8, 9,10,   8,10,11,
    // Left Face
    12,13,14,  12,14,15,
    // Top Face
    16,17,18,  16,18,19,
    // Bottom Face
    20,21,22,  20,22,23
};

int main(int argc, char* argv[]) {

    // Define matrices and set into uniforms
    mat4 model = mat4_construct_diagonal(1.0f);

    vec3 camera_pos      = (vec3){4.0f, 3.0f, 6.0f};
    mat4 view = mat4_lookat(camera_pos, (vec3){0.0f, 0.0f, 0.0f});

    mat4 projection = mat4_projection(0.1f, 1000.0f, 45.0f, (float)WIDTH/HEIGHT);

    context ctx;
    context_default_initialize(&ctx);

    // Create shader values for uniform buffer slots
    sv_create(model, mat4,      &ctx.uniform_buffer[UNIFORM_SLOT_1]);
    sv_create(view, mat4,       &ctx.uniform_buffer[UNIFORM_SLOT_2]);
    sv_create(projection, mat4, &ctx.uniform_buffer[UNIFORM_SLOT_3]);

    sv_create(mat4_mult_multiple(2, view, model), mat4, &ctx.uniform_buffer[UNIFORM_SLOT_4]);
    sv_create(mat4_mult_multiple(3, projection, view, model), mat4, &ctx.uniform_buffer[UNIFORM_SLOT_5]);

    sv_create(camera_pos, vec3, &ctx.uniform_buffer[UNIFORM_SLOT_6]);

    // Set viewport size
    context_set_viewport_size(&ctx, WIDTH, HEIGHT);

    texture container_tex;
    texture_load_texture(&container_tex, "container.jpg", true);

    framebuffer basic;
    framebuffer_construct(&basic, WIDTH, HEIGHT);

    ctx.out_buffer = &basic;

    ctx.vertex_buffer = vertices;
    ctx.index_buffer = indices;

    ctx.shader = (shader_program){.vertex_shader = &default_vert_shader, .fragment_shader = &blinn_phong_frag_shader};

    ctx.textures[TEXTURE_SLOT_1] = &container_tex;

    point_light light;
    p_light_default_construct(&light);
    light.position = (vec3){3.0f, 4.0f, 4.0f};
    light.intensity = 3.0f;
    p_light_atten_from_radius(&light, 10.0f);

    ctx.point_lights[0] = light;

    material random_mat;
    material_default_initialize(&random_mat);
    random_mat.specular = 5.0f;
    random_mat.shininess = 8.0f;

    ctx.material = random_mat;

    context_clear_colour(&ctx, (vec4){0.0f, 0.0f, 0.0f});
    context_clear_depth(&ctx);

    draw_indexed_triangles(&ctx, 36);

    context_output_image_ppm(&ctx, "image.ppm");

    // Cleanup context and destroy objects
    context_cleanup(&ctx);
    framebuffer_destroy(&basic);
    texture_destroy(&container_tex);

    return 0;
}
