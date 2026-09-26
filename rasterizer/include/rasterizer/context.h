#ifndef CONTEXT_H
#define CONTEXT_H

#include "vertex.h"
#include "shader_value.h"
#include "shader.h"
#include "framebuffer.h"
#include "math/vec4.h"
#include "light.h"
#include "material.h"
#include "texture.h"

#include <stdint.h>
#include <stdio.h>

#define MAX_UNIFORMS 16
typedef enum {
    UNIFORM_SLOT_1,
    UNIFORM_SLOT_2,
    UNIFORM_SLOT_3,
    UNIFORM_SLOT_4,
    UNIFORM_SLOT_5,
    UNIFORM_SLOT_6,
    UNIFORM_SLOT_7,
    UNIFORM_SLOT_8,
    UNIFORM_SLOT_9,
    UNIFORM_SLOT_10,
    UNIFORM_SLOT_11,
    UNIFORM_SLOT_12,
    UNIFORM_SLOT_13,
    UNIFORM_SLOT_14,
    UNIFORM_SLOT_15,
    UNIFORM_SLOT_16,
} uniform_slots;

#define MAX_DIRECTIONAL_LIGHTS 1
#define MAX_POINT_LIGHTS 8

#define MAX_TEXTURES 2
typedef enum {
    TEXTURE_SLOT_1,
    TEXTURE_SLOT_2
} texture_slots;
typedef struct context {

    vertex* vertex_buffer;
    uint32_t* index_buffer;

    shader_value uniform_buffer[MAX_UNIFORMS];

    directional_light dir_lights[MAX_DIRECTIONAL_LIGHTS];
    point_light point_lights[MAX_POINT_LIGHTS];

    shader_program shader;
    material material;

    texture* textures[MAX_TEXTURES];
    framebuffer* out_buffer;

    int vp_width;
    int vp_height;

} context;

void context_default_initialize(context* c);

void context_set_viewport_size(context* c, int w, int h);

void context_set_uniform_array(context* c, shader_value* uniforms, size_t count);

void context_clear_colour(const context* c, colour4 colour);

void context_clear_depth(const context* c);

void context_write_depth(const context* c, int x, int y, float depth);

void context_write_output(const context* c, int x, int y, colour4 colour);

void context_output_image_ppm(context* c, const char* file_name);

// Cleanup context object, frees shader values stored in uniform buffer
void context_cleanup(context* c);

#endif