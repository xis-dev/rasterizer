#include "shader.h"

vertex default_vert_shader(const context *ctx, vertex v) {

    mat4 mvp_matrix;
    sv_get_value(ctx->uniform_buffer[UNIFORM_SLOT_5], &mvp_matrix);

    v.pos =  mat4_transform(mvp_matrix, v.pos);

    return v;
}
