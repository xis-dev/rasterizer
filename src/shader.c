#include "shader.h"

#include "context.h"
#include "shader_value.h"

vertex default_vert_shader(const context *ctx, vertex v) {

    mat4 model_matrix;
    mat4 mvp_matrix;

    sv_get_value(ctx->uniform_buffer[UNIFORM_SLOT_1], &model_matrix);
    sv_get_value(ctx->uniform_buffer[UNIFORM_SLOT_5], &mvp_matrix);

    v.varyings.world_pos = vec3_convert_vec4(mat4_transform(model_matrix, v.pos));
    v.pos =  mat4_transform(mvp_matrix, v.pos);
    v.normal = mat3_transform(
            mat3_get_transpose(mat3_get_inverse(mat3_convert_mat4(model_matrix))),
               v.normal);

    return v;
}

colour4 blinn_phong_frag_shader(const context* ctx, vertex* v) {

    vec3 frag_to_light = vec3_sub(ctx->point_lights[0].position, v->varyings.world_pos);
    float dist_to_light = vec3_magnitude(frag_to_light);

    vec3_normalize_mut(&frag_to_light);
    
    float ambient = ctx->material.ambient;

    vec3 diffuse_color;
    sv_get_value(ctx->uniform_buffer[UNIFORM_SLOT_7], &diffuse_color);

    vec3 diffuse = vec3_scale(diffuse_color, ctx->material.diffuse);
    float specular;

    colour4 out = (colour4){v->pos.x / vec4_magnitude(v->pos), v->pos.y / vec4_magnitude(v->pos), v->pos.z / vec4_magnitude(v->pos), 1.0f};
    return out;
}
