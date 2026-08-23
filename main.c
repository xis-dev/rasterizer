#include <stdio.h>
#include <math.h>

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

    int x = s_pos1.x;
    int y = s_pos1.y;
    int y_mut = y;
    int dx = (int)s_pos2.x - x;
    int dy = (int)s_pos2.y - y;

    if (dx == 0) return;

    int m = dy / dx;
    float k = (float)dy / dx;
    float b = y - (k * x);

    // Choose lower x for start to goal
    int s_x = x < (int)s_pos2.x ? x : (int)s_pos2.x;
    int g_x = s_x == x ? (int)s_pos2.x : x;


    for (int x_it = s_x; x_it <= g_x; ++x_it) {
	out_buffer[x_it][(int)round(y_mut)] = line_col;
	y_mut += b;
    }

    out_buffer[x][y] = vec3_construct_sep(0.0, 0.0, 1.0);
    out_buffer[x + dx][y + dy] = vec3_construct_sep(0.0, 0.0, 1.0);
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
