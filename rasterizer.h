#ifndef RASTERIZER_H
#define RASTERIZER_H

#include "rasmath.h"

#include <stdlib.h>
#include <math.h>

typedef struct vertex {
    vec3 pos;
    colour3 col;
} vertex;


vec3 ndc_to_screen(vec3 ndc, int screen_w, int screen_h);

/** Rasterizer to draw the pixels of some buffer +/ image
 * '_make' signifies the function performs on multiple vertices and ordering and drawing shape based on the function
 * '_draw' signifies singular shape drawing
 * */
typedef struct rasterizer {
    int w, h;
    vec3* framebuffer;
    char* out_img;

} rasterizer;

typedef enum rasterizer_draw_mode {DRAW_LINES, DRAW_LINES_LOOP, DRAW_TRIANGLES} rasterizer_draw_mode;

rasterizer rasterizer_construct(int w, int h, char* out_img);

void rasterizer_clear_colour(rasterizer* r, colour3 col);

void rasterizer_resize_buffer(int w, int h);

void rasterizer_draw_pixel(rasterizer* r, int x, int y, colour3 c);

vec3 rasterizer_get_pixel(rasterizer* r, int x, int y);

void rasterizer_draw_line(rasterizer* r, vertex vt1, vertex vt2);

void rasterizer_make_lines_loop(rasterizer* r, size_t vc, vertex vs[]);

void rasterizer_draw_triangle(rasterizer* r, vertex v1, vertex v2, vertex v3);

void rasterizer_make_triangles(rasterizer* r, size_t vc, vertex vs[]);

void rasterizer_draw(rasterizer* r, rasterizer_draw_mode mode, size_t vc, vertex vs[]);


vec3 rasterizer_fb_at(int x, int y);

void rasterizer_output_ppm(rasterizer* r);

void rasterizer_delete(rasterizer* r);
#endif
