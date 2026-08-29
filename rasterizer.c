#include "rasterizer.h"

#include "rasmath.h"

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

    // Take larger axis
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
    rasterizer_draw_line(r, vs[vc - 1], vs[0]);

    for (int i = 0; i < vc - 1; ++i) {

	rasterizer_draw_line(r, vs[i], vs[i+1]);

    }
}

// Edge function for 3 counter clockwise described vertices, set to return a positive value
float edgeFunction(vec2 v1, vec2 v2, vec2 v3) {
    return (v3.x - v2.x) * (v2.y - v1.y) - (v3.y - v2.y) * (v2.x - v1.x);
}

// Fast compute barycentric when all areas are known
vec3 compute_barycentric_fast(float a1, float a2, float total_area) {

    vec3 out;

    out.x = a1 / total_area;
    out.y = a2 / total_area;
    out.z = 1.0 - out.x - out.y;

    return out;
}

// Compute barycentric of a point from triangle vertices
// TODO: Will need to change or make new ver for barycentric from randomly positioned triangles in 3d space, dropping the axis closest to the normal and such
vec3 compute_barycentric_screen(vec2 p, vec2 v1, vec2 v2, vec2 v3) {

    float p_area = edgeFunction(v1, v2, v3);

    float e0 = edgeFunction(v1, v2, p);
    float e1 = edgeFunction(v2, v3, p);

    return compute_barycentric_fast(e0, e1, p_area);
}

void rasterizer_draw_triangle(rasterizer* r, vertex vtx1, vertex vtx2, vertex vtx3) {

    vec2 ps[3];

    ps[0] = vec3_convert_vec2(ndc_to_screen(vtx1.pos, r->w, r->h));
    ps[1] = vec3_convert_vec2(ndc_to_screen(vtx2.pos, r->w, r->h));
    ps[2] = vec3_convert_vec2(ndc_to_screen(vtx3.pos, r->w, r->h));

    int xMin = ps[0].x;
    int yMin = ps[0].y;
    int xMax = ps[0].x;
    int yMax = ps[0].y;


    // Find bounding box
    for (int i = 1; i < 3; ++i) {
	if (ps[i].x < xMin) xMin = ps[i].x;
	if (ps[i].y < yMin) yMin = ps[i].y;
	if (ps[i].x > xMax) xMax = ps[i].x;
	if (ps[i].y > yMax) yMax = ps[i].y;
    }

    float p_area = edgeFunction(ps[0], ps[1], ps[2]);

    for (int y = yMin; y <= yMax; ++y) {

	for (int x = xMin; x <= xMax; ++x) {

	    vec2 p = (vec2){x, y};

	    float e0 = edgeFunction(ps[0], ps[1], p);
	    float e1 = edgeFunction(ps[1], ps[2], p);
	    float e2 = edgeFunction(ps[2], ps[0], p);

	    // Compute barycentric using ratio between parallelograms formed by main and inner triangles 
	    vec3 b = compute_barycentric_fast(e1, e2, p_area);

	    // if all edge functions fall within the positive plane, the point must be in the triangle
	    if (e0 >= 0 && e1 >= 0 && e2 >= 0) {
		colour3 c = vec3_add(vec3_add(vec3_scale(vtx1.col, b.x), vec3_scale(vtx2.col, b.y)), vec3_scale(vtx3.col, b.z));

		rasterizer_draw_pixel(r, x, y, c);
	    } 
	    
	}
    }
}

void rasterizer_make_triangles(rasterizer *r, size_t vc, vertex vs[]) {

    size_t verts_remaining = vc;
    size_t i = 0;

    while (verts_remaining >= 3) {

	// Draw triangle with 3 points
	rasterizer_draw_triangle(r, vs[i], vs[i + 1], vs[i + 2]);
	i += 3;
	verts_remaining -= 3;
    }

    // Not given enough vertices to create only triangles
    if (verts_remaining == 1) {

	// Draw singular pixel with last point
	vertex last_v = vs[vc - 1];
	rasterizer_draw_pixel(r, last_v.pos.x, last_v.pos.y, last_v.col);

    }
    else if (verts_remaining == 2) {
	// Draw line with last two points
	rasterizer_draw_line(r, vs[vc - 2], vs[vc - 1]);
    }
}

void rasterizer_draw(rasterizer* r, rasterizer_draw_mode mode, size_t vc, vertex vs[]) {

    switch(mode) {

	case DRAW_LINES:
	case DRAW_LINES_LOOP:
	    rasterizer_make_lines_loop(r, vc, vs);
	    break;
	case DRAW_TRIANGLES:
	    rasterizer_make_triangles(r, vc, vs);
	    break;
    }

    rasterizer_output_ppm(r);
}

void rasterizer_output_ppm(rasterizer* r) {

    FILE* img;

    img = fopen(r->out_img, "w");

    if (!img) {
	printf("Rasterizer failed to open ppm output file '%s' for writing", r->out_img);

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
