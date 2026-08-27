#include "rasterizer.h"
#include "vec2.h"
#include "vec3.h"

#include <stdio.h>
#include <string.h>

vec3 ndc_to_screen(vec3 ndc, int screen_w, int screen_h) {

    // Normalize ndc from [-1, 1] to [0, 1]
    vec3 ndc_norm = vec3_scale(ndc, 0.5);
    vec3_add_mut(&ndc_norm, vec3_construct(0.5));

    vec3 out;
    out.x = (double)trunc(ndc_norm.x * (screen_w - 1));
    out.y = (double)trunc(ndc_norm.y * (screen_h - 1));
    out.z = ndc.z;

    return out;
}
rasterizer rasterizer_construct(int w, int h, char* out_img) {

    rasterizer r;
    r.w = w;
    r.h = h;
    r.framebuffer = (vec3*)malloc(w * h * sizeof(vec3));

    r.out_img = (char*)malloc(strlen(out_img) + 1);
    strcpy(r.out_img, out_img);


    return r;
}

void rasterizer_clear_colour(rasterizer* r, colour3 col) {

    size_t buffer_size = r->w * r->h;
    for (size_t i = 0; i < buffer_size; ++i) {
	r->framebuffer[i] = col;
    }
}

void rasterizer_draw_pixel(rasterizer* r, int x, int y, colour3 c) {

    r->framebuffer[y * (r->w) + x] = c; 
}

vec3 rasterizer_get_pixel(rasterizer* r, int x, int y) {

    return r->framebuffer[y * (r->w) + x];
}

void rasterizer_draw_line(rasterizer* r, vertex vt1, vertex vt2) {

    vec3 s_pos_1 = ndc_to_screen(vt1.pos, r->w, r->h);
    vec3 s_pos_2 = ndc_to_screen(vt2.pos, r->w, r->h);

    vec2 v1 = (vec2){s_pos_1.x, s_pos_1.y};
    vec2 v2 = (vec2){s_pos_2.x, s_pos_2.y};

    int dx = fabs(v2.x - v1.x);
    int dy = fabs(v2.y - v1.y);

    // Line drawn on same pixel
    if (dx == 0 && dy == 0) {
	rasterizer_draw_pixel(r, v1.x, v1.y, vt1.col);

	return;
    }
    // Get signage to move x and y
    int sx = v2.x >= v1.x ? 1 : -1;
    int sy = v2.y >= v1.y ? 1 : -1;

    float dist_sq = vec2_dist_sq(v1, v2);

    
    colour3 c;

    // Take axis with more samples
    if (dy > dx) {
	int d  = 2 * dx - dy;
	int d1 = 2 * dx;
	int d2 = 2 * (dx - dy);
	int x = v1.x;
	int y = v1.y + sy;

	rasterizer_draw_pixel(r, v1.x, v1.y, vt1.col);

	for (int i = 1; i <= dy; ++i, y += sy) {

	    if (d > 0) {
		d += d2;
		x += sx;
	    }
	    else d += d1;
	    
	    // TODO: Use integer based interpolation
	    float a = vec2_dist_sq((vec2){x, y}, v2)/dist_sq;

	    c = vec3_add(vec3_scale(vt2.col, (1.0 - a)), vec3_scale(vt1.col, a));

	    rasterizer_draw_pixel(r, x, y, c);
	}
    }
    else {
	int d  = 2 * dy - dx;
	int d1 = 2 * dy;
	int d2 = 2 * (dy - dx);
	int x = v1.x + sx;
	int y = v1.y;

	rasterizer_draw_pixel(r, v1.x, v1.y, vt1.col);

	for (int i = 1; i <= dx; ++i, x += sx) {

	    if (d > 0) {
		d += d2;
		y += sy;
	    }
	    else d += d1;
	    
	    // TODO: Use integer based interpolation
	    float a = vec2_dist_sq((vec2){x, y}, v2)/dist_sq;

	    c = vec3_add(vec3_scale(vt2.col, (1.0 - a)), vec3_scale(vt1.col, a));

	    rasterizer_draw_pixel(r, x, y, c);
	}

    }
}

void rasterizer_make_lines_loop(rasterizer* r, size_t vc, vertex vs[]) {

    if (vc == 0) return;
    if (vc == 1) {

	rasterizer_draw_pixel(r, vs[0].pos.x, vs[0].pos.y, vs[0].col);
	return;
    }

    // Draw line from last to first then continue

    // TODO: Might be better to take in vec3's into draw functions to avoid unecessary casting everytime
    rasterizer_draw_line(r, vs[vc - 1], vs[0]);

    for (int i = 0; i < vc - 1; ++i) {

	rasterizer_draw_line(r, vs[i], vs[i+1]);

    }
}

void rasterizer_draw(rasterizer* r, rasterizer_draw_mode mode, size_t vc, vertex vs[]) {

    switch(mode) {

	case DRAW_LINES:
	    break;
	case DRAW_LINES_LOOP:
	    rasterizer_make_lines_loop(r, vc, vs);
	    break;
	case DRAW_TRIANGLES:
	    break;
    }

    rasterizer_output_ppm(r);
}

void rasterizer_output_ppm(rasterizer* r) {

    FILE* img;

    img = fopen(r->out_img, "w");

    if (!img) {
	printf("Rasterizer failed to open output file '%s' for writing", r->out_img);

	return;
    }

    fprintf(img, "P3 \n%i %i \n255\n", r->w, r->h);

    // Draw back colour first(TODO: Change to avoid double looping, or use clear func, or use depth buffer(probably not, precision issues)?)

    for (int y = 0; y < r->h; ++y) {

	for (int x = 0; x < r->w; ++x) {

	    colour3 col = rasterizer_get_pixel(r, x, y);

	    int r = col.x * 255.99;
	    int g = col.y * 255.99;
	    int b = col.z * 255.99;

	    fprintf(img,"%i %i %i\n", r, g, b);
	}
    }

    fclose(img);
}

void rasterizer_delete(rasterizer* r) {

    r->w = 0;
    r->h = 0;

    if (r->framebuffer) {
	free(r->framebuffer);

	r->framebuffer = NULL;
    }

    if (r->out_img) {

	free(r->out_img);

	r->out_img = NULL;
    }

}
