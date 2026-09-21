#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "vec4.h"

typedef struct {
    colour4* colour_buffer;
    float* depth_buffer;

    int width;
    int height;
} framebuffer;

void framebuffer_construct(framebuffer* f, int w, int h);

void framebuffer_destroy(framebuffer* f);
#endif