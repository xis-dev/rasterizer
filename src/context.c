#include "context.h"

void context_clear_colour(const context *c, colour4 colour) {

    size_t buff_size = c->vp_width * c->vp_height;

    for (size_t i = 0; i < buff_size; ++i) {
        c->out_buffer->colour_buffer[i] = colour;
    }
}

void context_clear_depth(const context *c) {
    // TODO: Add depth functions, changing depth clear value based on func

    size_t buff_size = c->vp_width * c->vp_height;

    for (size_t i = 0; i < buff_size; ++i) {
        c->out_buffer->depth_buffer[i] = 1.0f;
    }

}

void context_write_output(const context *c, int x, int y, colour4 colour) {

    c->out_buffer->colour_buffer[(c->vp_width * y) + x] = colour;
}
