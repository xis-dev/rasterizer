#include "shader.h"

#include <tgmath.h>

#include "context.h"
#include "shader_value.h"
#include "texture.h"
#include "rasmath.h"

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

    float light_intensity = 1.0f / (ctx->point_lights[0].attenuation.constant +
                                    ctx->point_lights[0].attenuation.linear * dist_to_light +
                                    ctx->point_lights[0].attenuation.linear * (dist_to_light * dist_to_light));
    light_intensity *= ctx->point_lights[0].intensity;

    vec3_normalize_mut(&frag_to_light);
    
    float ambient = ctx->material.ambient;

    vec3 diffuse_color;
    texture_read_into_colour(ctx->textures[0], (int)(v->uv_0.e[0] * ctx->textures[0]->width), (int)(v->uv_0.e[1] * ctx->textures[0]->height), (&diffuse_color));

    float diff_factor = fmax(fabsf(vec3_dot(frag_to_light, v->normal)), 0.0f);
    vec3 diffuse = vec3_scale(diffuse_color, ctx->material.diffuse * light_intensity * diff_factor);

    // TODO: Specular
    colour3 out_col = vec3_multiply(ctx->material.color, diffuse);

    return vec4_convert_vec3(diffuse_color, 1.0f);

}
