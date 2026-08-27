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


void buildPlane(point3 p1, point3 p2, point3 p3, 
		int* a, int* b, int* c) {

    // Line equations from p1 and p2
    *a = p2.y - p1.y; // -dy
    *b = p1.x - p2.x; // dx;
    *c = p2.x * p1.y - p2.y * p1.x;

    // Negate if 3rd vertex is negative, want to normalize positive numbers
    if (*a * p3.x + *b * p3.y + *c < 0) {
	*a = -(*a);
	*b = -(*b);
	*c = -(*c);
    }
}

// Edge function for 3 counter clockwise described vertices, set to return a positive value
double edgeFunction(point3 v1, point3 v2, point3 v3) {
    return (v3.x - v2.x) * (v2.y - v1.y) - (v3.y - v2.y) * (v2.x - v1.x);
}

void rasterizeTriangle(point3 ps[]) {

    int xMin = ps[0].x;
    int yMin = ps[0].y;
    int xMax = ps[0].x;
    int yMax = ps[0].y;

    int a[3], b[3], c[3];

    // Find bounding box
    for (int i = 1; i < 3; ++i) {
	if (ps[i].x < xMin) xMin = ps[i].x;
	if (ps[i].y < yMin) yMin = ps[i].y;
	if (ps[i].x > xMax) xMax = ps[i].x;
	if (ps[i].y > yMax) yMax = ps[i].y;
    }

    // Build planes for each edge
    buildPlane(ps[0], ps[1], ps[2], &a[0], &b[0], &c[0]);
    buildPlane(ps[1], ps[2], ps[0], &a[1], &b[1], &c[1]);
    buildPlane(ps[2], ps[0], ps[1], &a[2], &b[2], &c[2]);

    int d0 = a[0] * xMin + b[0] * yMin + c[0];
    int d1 = a[1] * xMin + b[1] * yMin + c[1];
    int d2 = a[2] * xMin + b[2] * yMin + c[2];

    for (int y = yMin; y <= yMax; ++y) {

	int f0 = d0;
	int f1 = d1;
	int f2 = d2;

	d0 += b[0];
	d1 += b[1];
	d2 += b[2];

	for (int x = xMin; x <= xMax; ++x) {

	    // if all f's fall within the positive plane, the point must be in the triangle
	    if (f0 >= 0 && f1 >= 0 && f2 >= 0) out_buffer[x][y] = vec3_construct_sep(0.2, 0.4, 1.0);

	    f0 += a[0];
	    f1 += a[1];
	    f2 += a[2];
	    
	    
	}
    }

}
// Currently only ndc vertices + connect lines
int draw(int w, int h, colour3 back_colour, size_t vc, vertex vs[], char* output_img) {


    return 0;
}

int main(int argc, char* argv[]) {

    vertex vertices[3] = {
	{(vec3){0.5, 0.5, 0.0},(vec3){1.0, 0.0, 0.0}},
	{(vec3){ 0.0, -0.5, 0.0},(vec3){0.0, 1.0, 0.0}},
	{(vec3){-0.5, 0.5, 0.0}, (vec3){0.0, 0.0, 1.0}}
    };

    rasterizer r;

    r = rasterizer_construct(WIDTH, HEIGHT, "image.ppm");

    rasterizer_clear_colour(&r, (vec3){0.0, 0.0, 0.0});
    rasterizer_draw(&r, DRAW_LINES_LOOP, 3, vertices);

    rasterizer_delete(&r);
    return 0;
}
