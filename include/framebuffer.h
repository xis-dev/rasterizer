#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "vec4.h"

typedef struct {
    colour4* colour_buffer;
    float* depth_buffer;
} framebuffer;

void framebuffer_construct(framebuffer* f, int w, int h);


#endif