#include "shader.h"

#include "context.h"
#include "shader_value.h"

vertex default_vert_shader(const context *ctx, vertex v) {

    mat4 model_matrix;
    mat4 mvp_matrix;

    sv_get_value(ctx->uniform_buffer[UNIFORM_SLOT_1], &model_matrix);
    sv_get_value(ctx->uniform_buffer[UNIFORM_SLOT_5], &mvp_matrix);

    v.pos =  mat4_transform(mvp_matrix, v.pos);
    v.normal = mat3_transform(
            mat3_get_transpose(mat3_get_inverse(mat3_convert_mat4(model_matrix))),
               v.normal);


    return v;
}

colour4 default_frag_shader(const context* ctx, vertex* v) {
    colour4 out = (colour4){v->pos.x / vec4_magnitude(v->pos), v->pos.y / vec4_magnitude(v->pos), v->pos.z / vec4_magnitude(v->pos), 1.0f};
    return out;
}
