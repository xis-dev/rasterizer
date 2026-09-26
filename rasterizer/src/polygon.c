#include "../include/rasterizer/polygon.h"

#include <string.h>

void polygon_construct(polygon *p) {
    p->count = 0;
}

vertex* polygon_vertex_at(polygon *p, uint8_t idx) {

    if (idx >= p->count) return NULL;

    return &p->data[idx];
}

void polygon_add_vertex(polygon *p, vertex v) {

    p->data[p->count] = v;
    ++p->count;
}

void polygon_lazy_clear(polygon *p) {
    p->count = 0;
}

void polygon_deep_clear(polygon *p) {
    memset(p->data, 0, sizeof(vertex) * p->count);
    p->count = 0;
}

polygon polygon_triangulate(polygon p) {
    if (p.count < 3) return p;

    polygon out;
    polygon_construct(&out);
    const vertex v1 = p.data[0];

    for (int v = 2; v < p.count; ++v) {

        polygon_add_vertex(&out, v1);
        polygon_add_vertex(&out, p.data[v - 1]);
        polygon_add_vertex(&out, p.data[v]);
    }

    return out;
}
