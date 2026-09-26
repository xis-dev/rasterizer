#include "../include/rasterizer/framebuffer.h"

#include <stdlib.h>

void framebuffer_construct(framebuffer *f, int w, int h) {

    f->colour_buffer = (colour4*) calloc(w * h, sizeof(colour4));
    f->depth_buffer  = (float*)   calloc(w * h, sizeof(float));

    f->width = w;
    f->height = h;
}

void framebuffer_destroy(framebuffer *f) {
    free(f->colour_buffer);
    free(f->depth_buffer);
}
