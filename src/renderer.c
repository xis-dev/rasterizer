#include "renderer.h"

void process_source_triangle(const context *ctx, vertex v1, vertex v2, vertex v3) {

    v1 = ctx->shader->vertex_shader(ctx, v1);
    v2 = ctx->shader->vertex_shader(ctx, v2);
    v3 = ctx->shader->vertex_shader(ctx, v3);

    // TODO: Add culling, pre clip
    polygon clipped = clip_triangle(v1, v2, v3);

    // Triangle was fully clipped out or ill-formed
    if (clipped.count < 3) return;


}

// Edge function for a set of clockwise vertices returning positive if on the right side of plane
static float edge_function(vec4 a, vec4 b, vec4 c) {
    return (c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x);
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

            in = out;
        }
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
