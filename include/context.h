#ifndef CONTEXT_H
#define CONTEXT_H

#include "vertex.h"
#include "shader_value.h"
#include "shader.h"
#include "framebuffer.h"
#include "vec4.h"

#include <stdint.h>
#include <stdio.h>

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

typedef struct context {

    vertex* vertex_buffer;
    uint32_t* index_buffer;

    shader_value* uniform_buffer;

    shader_program* shader;

    framebuffer* out_buffer;

    int vp_width;
    int vp_height;

} context;

void context_clear_colour(const context* c, colour4 colour);

void context_clear_depth(const context* c);

void context_write_output(const context* c, int x, int y, colour4 colour);

#endif