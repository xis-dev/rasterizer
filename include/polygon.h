#ifndef POLYGON_H
#define POLYGON_H

#include "vertex.h"

#include "stdint.h"

#define MAX_CLIPPED_VERTICES 9

typedef struct {
    vertex data[MAX_CLIPPED_VERTICES];
    uint8_t count;
} polygon;

void polygon_construct(polygon* p);

vertex* polygon_vertex_at(polygon* p, uint8_t idx);

void polygon_add_vertex(polygon* p, vertex v);

void polygon_lazy_clear(polygon* p);

void polygon_deep_clear(polygon* p);

polygon polygon_triangulate(polygon p);

#endif