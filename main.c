#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "vec3.h"

#define WIDTH 1600
#define HEIGHT 900

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

    int sx = xb >= xa ? 1 : -1; // sign of xb - xa;
    int sy = yb >= ya ? 1 : -1; // sign of yb - ya;

    if (dy <= dx) {
	int d  = 2 * dy - dx;
	int d1 = 2 * dy;
	int d2 = 2 * (dy - dx);
	int x = xa + sx;
	int y = ya;

	out_buffer[xa][ya] = line_col;
	for (int i = 1; i <= dx; ++i, x += sx) {
	    if (d > 0) {
		
		d += d2;
		y += sy;
	    }
	    else d += d1;

	    out_buffer[x][y] = line_col;
	}
    }
    else {

	int d  = 2 * dx - dy;
	int d1 = 2 * dx;
	int d2 = 2 * (dx - dy);
	int x = xa;
	int y = ya + sy;

	out_buffer[x][y] = line_col;
	for (int i = 1; i <= dy; ++i, y += sy) {
	    if (d > 0) {
		d += d2;
		x += sx;
	    }
	    else d += d1;

	    out_buffer[x][y] = line_col;
	}

    }
    out_buffer[xa][ya] = vec3_construct_sep(0.0, 1.0, 0.0);
}

// Currently only ndc vertices + connect lines
int draw(int w, int h, vec3 back_colour, size_t vc, vertex vs[], char* output_img) {
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
    draw_line(vs[vc - 1].pos, vs[0].pos, (vec3){0.0, 1.0, 0.0});

    for (int i = 0; i < vc - 1; ++i) {
	draw_line(vs[i].pos, vs[i+1].pos, (vec3){0.0, 1.0, 0.0});
    }

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
