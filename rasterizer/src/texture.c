#include "../include/rasterizer/texture.h"

#include "stb/stb_image.h"

void texture_load_texture(texture* tex_out, const char* filename, bool flip_on_load) {

    stbi_set_flip_vertically_on_load(flip_on_load);

    tex_out->data = stbi_load(filename, &tex_out->width, &tex_out->height, &tex_out->channels, 0);

    if (!tex_out->data) {
        printf("Failed to load image: %s", filename);
        return;
    }

    stbi_set_flip_vertically_on_load(false);
}

colour4 texture_get_normalized_float4_array(texture* tex, float uv_x, float uv_y) {
    colour4 out = {0};

    float one_over_255 = 1.0f / 255.0f;
    uint8_t* tex_values = texture_read(tex, (int)(uv_x * (float)tex->width), (int)(uv_y * (float)tex->height));
    for (int i = 0; i < tex->channels; ++i) {
        out.e[i] = (float)tex_values[i] * one_over_255;
    }

    return out;
}

uint8_t* texture_read(texture* tex, int x, int y) {

    // Repeat texture if uv out of range
    x = x < 0 ? abs(x) : x;
    y = y < 0 ? abs(y) : y;

    x = x >= tex->width  ? x % tex->width  : x;
    y = y >= tex->height ? y % tex->height : y;

    return &(tex->data[((y * tex->width) + x) * tex->channels]);
}

void texture_destroy(texture* tex) {

    stbi_image_free(tex->data);

    *tex = (texture){0};
}
