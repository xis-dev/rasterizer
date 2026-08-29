#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <threads.h>

#include "rasterizer.h"
#include "vec3.h"

#define WIDTH 160
#define HEIGHT 90

// Row-array to hold column array
vec3 out_buffer[WIDTH][HEIGHT];


// Currently only ndc vertices + connect lines
int draw(int w, int h, colour3 back_colour, size_t vc, vertex vs[], char* output_img) {


    return 0;
}

int main(int argc, char* argv[]) {

    vertex vertices[5] = {
	{(vec3){0.5, 0.5, 0.0},(vec3){1.0, 0.0, 0.0}},
	{(vec3){ 0.0, -0.5, 0.0},(vec3){0.0, 1.0, 0.0}},
	{(vec3){-0.5, 0.5, 0.0}, (vec3){0.0, 0.0, 1.0}},
	{(vec3){-0.5, 0.5, 0.0}, (vec3){0.0, 0.0, 1.0}},
	{(vec3){-1.0, -0.5, 0.0}, (vec3){0.0, 1.0, 0.0}}
    };

    rasterizer r;

    r = rasterizer_construct(WIDTH, HEIGHT, "image.ppm");

    rasterizer_clear_colour(&r, (vec3){0.0, 0.0, 0.0});
    rasterizer_draw(&r, DRAW_TRIANGLES, 5, vertices);

    rasterizer_delete(&r);
    return 0;
}
