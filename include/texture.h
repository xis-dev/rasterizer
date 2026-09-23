#ifndef TEXTURE_H
#define TEXTURE_H

#include <stdint.h>

#include "vec2.h"
#include "vec4.h"

typedef struct {

    int width;
    int height;
    int channels;

    uint8_t* data;
}texture;

void texture_load_texture(texture* tex_out, const char* filename, bool flip_on_load);

uint8_t* texture_read(texture* tex, int x, int y);

// Return texture elements as an array of floats ranging from [0,1]
colour4 texture_get_normalized_float4_array(texture* tex, int x, int y);

static inline void texture_fill_2(texture* tex, int x, int y, colour2* c) {
    colour4 e = texture_get_normalized_float4_array(tex, x, y);
    c->e[0] = e.e[0]; c->e[1] = e.e[1];
}

static inline void texture_fill_3(texture* tex, int x, int y, colour3* c) {
    colour4 e = texture_get_normalized_float4_array(tex, x, y);
    c->e[0] = e.e[0]; c->e[1] = e.e[1]; c->e[2] = e.e[2];
}

static inline void texture_fill_4(texture* tex, int x, int y, colour4* c) {
    colour4 e = texture_get_normalized_float4_array(tex, x, y);
    c->e[0] = e.e[0]; c->e[1] = e.e[1]; c->e[2] = e.e[2]; c->e[3] = e.e[3];
}

#define texture_read_into_colour(tex_in_ptr, pos_x, pos_y, col_out) \
    do {\
    _Generic((col_out), \
    colour2*: texture_fill_2,\
    colour3*: texture_fill_3,\
    colour4*: texture_fill_4 \
) (tex_in_ptr, pos_x, pos_y, col_out); \
} while(0)


void texture_destroy(texture* tex);

#endif
