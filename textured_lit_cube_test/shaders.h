#ifndef SHADERS_H
#define SHADERS_H

#include <rasterizer/shader.h>

vertex default_vert_shader(const context* ctx, vertex v);
colour4 blinn_phong_frag_shader(const context* ctx, vertex* v);

colour4 default_triangle_frag(const context* ctx, vertex* v);

#endif
