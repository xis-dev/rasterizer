#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <threads.h>

#include "vec3.h"

#define WIDTH 160
#define HEIGHT 90

// Row-array to hold column array
vec3 out_buffer[WIDTH][HEIGHT];

typedef struct vertex {
    vec3 pos;
} vertex;

vec3 ndc_to_screen(vec3 ndc) {
    vec3 ndc_norm = vec3_scale(ndc, 0.5);
    vec3_add_mut(&ndc_norm, vec3_construct(0.5));

    vec3 out;
    out.x = (double)trunc(ndc_norm.x * (WIDTH - 1));
    out.y = (double)trunc(ndc_norm.y * (HEIGHT - 1));
    out.z = ndc.z;

    return out;
}

void draw_line(vec3 ndc_pos1, vec3 ndc_pos2, vec3 line_col) {

    vec3 s_pos1 = ndc_to_screen(ndc_pos1);
    vec3 s_pos2 = ndc_to_screen(ndc_pos2);

    int xa = s_pos1.x;
    int ya = s_pos1.y;

    int xb = s_pos2.x;
    int yb = s_pos2.y;

    int dx = abs(xb - xa);
    int dy = abs(yb - ya);

    int sx = xb >= xa ? 1 : -1;
    int sy = yb >= ya ? 1 : -1;

    // Distance on the x-axis is longer, use x to "move"
    // "k" = dy/dx when x dist >= y dist, otherwise 'k' = dx/dy
    // NOTE: Importance of using the higher distance/greater diff, stripping d back to k, method uses 1 axis to base the value of the other, without enough samples, results will be improper + can result in division by zero(if we were still dividing)
    if (dy <= dx) {
	int d  = 2 * dy - dx;
	int d1 = 2 * dy;
	int d2 = 2 * (dy - dx);
	int x = xa + sx;
	int y = ya;

	out_buffer[xa][ya] = line_col;
	// Start at next pixel from start, x and y move (B relative to A) p
	for(int i = 1; i <= dx; ++i, x += sx) {
	    if (d > 0) {
		
		d += d2;
		// Move y in the difference direction
		y += sy;
	    }
	    else d += d1;

	    out_buffer[x][y] = line_col;
	}
    }
    else {
	// k = dx/dy
	int d  = 2 * dx - dy;
	int d1 = 2 * dx;
	int d2 = 2 * (dx - dy);
	int x = xa;
	int y = ya + sy;

	out_buffer[xa][ya] = line_col;
	// Start at next pixel from start, x and y move (B relative to A) p
	for(int i = 1; i <= dy; ++i, y += sy) {
	    if (d > 0) {
		
		d += d2;
		// Move x in the difference direction
		x += sx;
	    }
	    else d += d1;

	    out_buffer[x][y] = line_col;
	}
    }
    out_buffer[xa][ya] = vec3_construct_sep(0.0, 1.0, 0.0);
}

void buildPlane(point3 p1, point3 p2, point3 p3, 
		int* a, int* b, int* c) {

    // Find line equations from p1 and p2
    *a = p2.y - p1.y;
    *b = p1.x - p2.x;
    *c = p1.x * p2.y - p1.y * p2.x;

    // Check whether p3 is in positive
    if (*a * p3.x + *b * p3.y + *c < 0) {
	*a = -(*a);
	*b = -(*b);
	*c = -(*c);
    }


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

    // Build line equations
    buildPlane(ps[0], ps[1], ps[2], &a[0], &b[0], &c[2]);
    buildPlane(ps[0], ps[2], ps[1], &a[1], &b[1], &c[1]);
    buildPlane(ps[1], ps[2], ps[0], &a[2], &b[2], &c[2]);


    // find functions at lower-left corner
    int d0 = a[0] * xMin + b[0] * yMin + c[0];
    int d1 = a[1] * xMin + b[1] * yMin + c[1];
    int d2 = a[2] * xMin + b[2] * yMin + c[2];

    // Check points
    for (int y = yMin; y <= yMax; ++y) {
	int f0 = d0;
	int f1 = d1;
	int f2 = d2;

	d0 += b[0];
	d1 += b[1];
	d2 += b[2];

	for (int x = xMin; x <= xMax; ++x) {
	    if (f0 >= 0 && f1 >= 0 && f2 >= 0) {
		out_buffer[x][y] = vec3_construct_sep(1.0, 1.0, 1.0);

		f0 += a[0];
		f1 += a[1];
		f2 += a[2];
	    }
	}
    }

}
// Currently only ndc vertices + connect lines
int draw(int w, int h, colour3 back_colour, size_t vc, vertex vs[], char* output_img) {
    FILE* f_out;

    f_out = fopen(output_img, "w");

    if (!f_out) {
	printf("Failed to open file '%s' for writing", output_img);
	return -1;
    }

    fprintf(f_out, "P3 \n%i %i \n255\n", w, h);

    // Write background colours
    for (int y = 0; y < h; ++y) {

	for (int x = 0; x < w; ++x) {
	    out_buffer[x][y] = back_colour;
	}
    }

    // Write lines between vertices 
    // Creating line loop, start on last vertex to first then loop from first to second to last
   // draw_line(vs[vc - 1].pos, vs[0].pos, (vec3){0.0, 1.0, 0.0});

    //for (int i = 0; i < vc - 1; ++i) {
//	draw_line(vs[i].pos, vs[i+1].pos, (vec3){0.0, 1.0, 0.0});
  //  }
  //
  point3 positions[3];

  for (int i = 0; i < 3; ++i) {
      positions[i] = ndc_to_screen(vs[i].pos);
  }
  rasterizeTriangle(positions);
    // Write to file and close
    for (int y = 0; y < h; ++y) {

	for (int x = 0; x < w; ++x) {

	    int r = out_buffer[x][y].x * 255.99;
	    int g = out_buffer[x][y].y * 255.99;
	    int b = out_buffer[x][y].z * 255.99;

	    fprintf(f_out,"%i %i %i\n", r, g, b);
	}
    }

    fclose(f_out);

    return 0;
}

int main(int argc, char* argv[]) {

    vertex vertices[3] = {
	{(vec3){-0.5, 0.5, 0.0}},
	{(vec3){ 0.0, -0.5, 0.0}},
	{(vec3){ 0.5, 0.5, 0.0}}
    };

    return draw(WIDTH, HEIGHT, (vec3){1.0, 0.0, 0.0}, 3, vertices, "image.ppm");
}
