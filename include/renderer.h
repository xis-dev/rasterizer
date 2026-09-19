#ifndef RENDERER_H
#define RENDERER_H

#include "context.h"
#include "vertex.h"
#include "polygon.h"

#include <stddef.h>

void process_source_triangle(const context* ctx, vertex v1, vertex v2, vertex v3);

polygon clip_triangle(vertex v1, vertex v2, vertex v3);

void rasterize_triangle(const context* ctx, vertex v1, vertex v2, vertex v3);

void draw_listed_triangles(const context* ctx, size_t vert_count);

void draw_indexed_triangles(const context* ctx, size_t index_count);
#endif