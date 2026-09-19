#include "renderer.h"

#include "math.h"
#include "stdbool.h"

void process_source_triangle(const context *ctx, vertex v1, vertex v2, vertex v3) {

    v1 = ctx->shader->vertex_shader(ctx, v1);
    v2 = ctx->shader->vertex_shader(ctx, v2);
    v3 = ctx->shader->vertex_shader(ctx, v3);

    // TODO: Add culling, pre clip
    polygon clipped = clip_triangle(v1, v2, v3);

    // Triangle was fully clipped out or ill-formed
    if (clipped.count < 3) return;

    for (int i = 0; i < clipped.count; i += 3) {

        float one_o_w0 = 1.0f / clipped.data[i].pos.w;
        clipped.data[i].pos.x *= one_o_w0;
        clipped.data[i].pos.y *= one_o_w0;
        clipped.data[i].pos.z *= one_o_w0;

        float one_o_w1 = 1.0f / clipped.data[i + 1].pos.w;
        clipped.data[i + 1].pos.x *= one_o_w1;
        clipped.data[i + 1].pos.y *= one_o_w1;
        clipped.data[i + 1].pos.z *= one_o_w1;

        float one_o_w2 = 1.0f / clipped.data[i + 2].pos.w;
        clipped.data[i + 2].pos.x *= one_o_w2;
        clipped.data[i + 2].pos.y *= one_o_w2;
        clipped.data[i + 2].pos.z *= one_o_w2;

        rasterize_triangle(ctx,
                           clipped.data[i],
                           clipped.data[i + 1],
                           clipped.data[i + 2]);
    }

}

static vec4 ndc_to_screen(vec4 ndc, int screen_w, int screen_h) {

    // Normalize ndc from [-1, 1] to [0, 1]
    vec4 out;

    out.x = (float)trunc((ndc.x * 0.5f + 0.5f) * (float)(screen_w - 1));
    out.y = (float)trunc((ndc.y * 0.5f + 0.5f) * (float)(screen_h - 1));
    out.z = ndc.z;
    out.w = ndc.w;

    return out;
}

// Edge function for a set of clockwise vertices returning positive if on the right side of plane
static float edge_function(vec2 a, vec2 b, vec2 c) {
    return (c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x);
}

// Compute perspective correct barycentric coordinates
/* 'area_x' corresponds to the area of the triangle opposite to the vertex specified by x
 *  area_1 = area of triangle formed without v1, formed by v2, v3 and p
 */
static vec3 compute_barycentric(vec4 v1, float area_1, vec4 v2, float area_2, vec4 v3, float area_3) {

    // Find areas in camera/eye space
    float area_1_eye = v2.w * v3.w * area_1;
    float area_2_eye = v3.w * v1.w * area_2;
    float area_3_eye = v1.w * v2.w * area_3;
    float total_area = area_1_eye + area_2_eye + area_3_eye;

    vec3 b_out;
    b_out.x = area_1_eye / total_area;
    b_out.y = area_2_eye / total_area;
    b_out.z = area_3_eye / total_area;

    return b_out;
}

static vertex get_bary_interpolated_vertex(vertex v1, vertex v2, vertex v3, vec3 b) {

    vertex out;

    out.pos = vec4_add(vec4_add(vec4_scale(v1.pos, b.x),
                                vec4_scale(v2.pos, b.y)),
                                vec4_scale(v3.pos, b.z));

    out.normal = vec3_add(vec3_add(vec3_scale(v1.normal, b.x),
                                   vec3_scale(v2.normal, b.y)),
                                   vec3_scale(v3.normal, b.z));

    out.uv_0 = vec2_add(vec2_add(vec2_scale(v1.uv_0, b.x),
                                 vec2_scale(v2.uv_0, b.y)),
                                 vec2_scale(v3.uv_0, b.z));

    out.uv_1 = vec2_add(vec2_add(vec2_scale(v1.uv_1, b.x),
                                 vec2_scale(v2.uv_1, b.y)),
                                 vec2_scale(v3.uv_1, b.z));

    return out;

}

// TODO: Depth functions
static bool depth_test(const context* c, int pos_x, int pos_y, float depth) {

    float current_depth = c->out_buffer->depth_buffer[pos_y * c->vp_width + pos_x];

    if (current_depth <= depth) return true;

    return false;
}

void rasterize_triangle(const context* ctx, vertex v1, vertex v2, vertex v3) {

    v1.pos = ndc_to_screen(v1.pos, ctx->vp_width, ctx->vp_height);
    v2.pos = ndc_to_screen(v2.pos, ctx->vp_width, ctx->vp_height);
    v3.pos = ndc_to_screen(v3.pos, ctx->vp_width, ctx->vp_height);

    int xMin = (int)v1.pos.x;
    int yMin = (int)v1.pos.y;
    int xMax = (int)v1.pos.x;
    int yMax = (int)v1.pos.y;


    // Find bounding box
    if ((int)v2.pos.x < xMin) xMin = (int)v2.pos.x;
    if ((int)v2.pos.y < yMin) yMin = (int)v2.pos.y;
    if ((int)v2.pos.x > xMax) xMax = (int)v2.pos.x;
    if ((int)v2.pos.y > yMax) yMax = (int)v2.pos.y;

    if ((int)v3.pos.x < xMin) xMin = (int)v3.pos.x;
    if ((int)v3.pos.y < yMin) yMin = (int)v3.pos.y;
    if ((int)v3.pos.x > xMax) xMax = (int)v3.pos.x;
    if ((int)v3.pos.y > yMax) yMax = (int)v3.pos.y;

    for (int y = yMin; y <= yMax; ++y) {

        for (int x = xMin; x <= xMax; ++x) {

            const vec2 p = (vec2){(float)x, (float)y};

            const vec2 v1_2D = vec2_convert_vec4(v1.pos);
            const vec2 v2_2D = vec2_convert_vec4(v2.pos);
            const vec2 v3_2D = vec2_convert_vec4(v3.pos);

            const float e1 = edge_function(v2_2D, v3_2D, p);
            const float e2 = edge_function(v3_2D, v1_2D, p);
            const float e3 = edge_function(v1_2D, v2_2D, p);


            // if all edge functions fall within the positive plane, the point must be in the triangle
            if (e1 >= 0 && e2 >= 0 && e3 >= 0) {
                // Compute barycentric using ratio between parallelograms formed by main and inner triangles
                vec3 b = compute_barycentric(v1.pos, e1, v2.pos, e2, v3.pos, e3);

                vertex fragment_vert = get_bary_interpolated_vertex(v1, v2, v3, b);

                // Ensure exact coords, incase of floating point creep
                fragment_vert.pos.x = (float)x;
                fragment_vert.pos.y = (float)y;

                colour4 frag_colour = ctx->shader->fragment_shader(ctx, &fragment_vert);

                if (depth_test(ctx, x, y, fragment_vert.pos.z)) {
                    context_write_output(ctx, x, y, frag_colour);
                }
            }

        }
    }
}

void draw_listed_triangles(const context *ctx, size_t vert_count) {
    for (size_t i = 0; i < vert_count; i += 3) {
        process_source_triangle(ctx,
                                ctx->vertex_buffer[i],
                                ctx->vertex_buffer[i + 1],
                                ctx->vertex_buffer[i + 2]);
    }
}

static vertex get_interpolated_vertex(vertex a, vertex b, float t) {

    vertex out;

    out.pos    = vec4_lerp(a.pos, b.pos, t);
    out.normal = vec3_lerp(a.normal, b.normal, t);
    out.uv_0   = vec2_lerp(a.uv_0, b.uv_0, t);
    out.uv_1   = vec2_lerp(a.uv_1, b.uv_1, t);

    return out;
}

polygon clip_triangle(vertex v1, vertex v2, vertex v3) {

    // Clip planes stored as 4D vectors representing the coefficients of the plane equation Ax + By + Cz + d
    vec4 clip_planes[6] = {
        (vec4){ 1.0f,  0.0f,  0.0f, 1.0f},  // left
        (vec4){-1.0f,  0.0f,  0.0f, 1.0f},  // right
        (vec4){ 0.0f, -1.0f,  0.0f, 1.0f},  // top
        (vec4){ 0.0f,  1.0f,  0.0f, 1.0f},  // bottom
        (vec4){ 0.0f,  0.0f,  1.0f, 0.0f},  // near
        (vec4){ 0.0f,  0.0f, -1.0f, 1.0f},  // far
    };

    polygon out;

    polygon in;
    polygon_construct(&in);
    polygon_add_vertex(&in, v1);
    polygon_add_vertex(&in, v2);
    polygon_add_vertex(&in, v3);

    // Test edges against each clip plane
    for (int p = 0; p < 6; ++p) {
        polygon_lazy_clear(&out);

        for (int v = 0; v < in.count; ++v) {
            // Get both vertices that form the edge(clockwise ordered)
            const vertex* a = &in.data[v];
            const vertex* b = &in.data[(v + 1) % in.count];

            const float da = vec4_dot(a->pos, clip_planes[p]);
            const float db = vec4_dot(b->pos, clip_planes[p]);

            if (da >= 0.0f && db >= 0.0f) { // Both vertices are inside, add only second vertex
                polygon_add_vertex(&out, *b);
            }
            else if (da >= 0.0f && db < 0.0f) { // First vertex inside, second outside, add only point of intersection

                const float t = da / (da - db);
                polygon_add_vertex(&out,get_interpolated_vertex(*a, *b, t));

            }
            else if (da < 0.0f && db >= 0.0f) { // First vertex outside, second inside, add point of intersection and second vertex

                const float t = da / (da - db);
                polygon_add_vertex(&out,get_interpolated_vertex(*a, *b, t));

                polygon_add_vertex(&out, *b);
            }
            else { // Both vertices are outside, do nothing
                continue;
            }

        }
        in = out;
    }

    return polygon_triangulate(out);
}

void draw_indexed_triangles(const context *ctx, size_t index_count) {
    for (size_t i = 0; i < index_count; i += 3) {
        process_source_triangle(ctx,
                                ctx->vertex_buffer[ctx->index_buffer[i]],
                                ctx->vertex_buffer[ctx->index_buffer[i + 1]],
                                ctx->vertex_buffer[ctx->index_buffer[i + 2]]);
    }
}
