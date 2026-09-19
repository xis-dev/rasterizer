#include "framebuffer.h"

#include <stdlib.h>

void framebuffer_construct(framebuffer *f, int w, int h) {

    f->colour_buffer = (colour4*) malloc(sizeof(colour4) * w * h);
    f->depth_buffer  = (float*)   malloc(sizeof(float) * w * h);

}
