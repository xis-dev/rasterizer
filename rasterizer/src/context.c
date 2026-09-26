#include "../include/rasterizer/context.h"

void context_default_initialize(context *c) {
    *c = (context){0};

}

void context_set_viewport_size(context* c, int w, int h) {

    c->vp_width = w;
    c->vp_height = h;
}

void context_clean_uniform_at(context *c, uniform_slots slot) {
    if (c->uniform_buffer[slot].value_type == UTYPE_NULL) return;

    free(c->uniform_buffer[slot].value);
    c->uniform_buffer[slot].value = NULL;
    c->uniform_buffer[slot].value_type = UTYPE_NULL;
}


void context_clear_colour(const context *c, colour4 colour) {

    size_t buff_size = c->vp_width * c->vp_height;

    for (size_t i = 0; i < buff_size; ++i) {
        c->out_buffer->colour_buffer[i] = colour;
    }
}

void context_clear_depth(const context *c) {
    // TODO: Add depth functions, changing depth clear value based on func

    size_t buff_size = c->vp_width * c->vp_height;

    for (size_t i = 0; i < buff_size; ++i) {
        c->out_buffer->depth_buffer[i] = 1.0f;
    }

}

void context_write_depth(const context *c, int x, int y, float depth) {
    c->out_buffer->depth_buffer[(c->vp_width * y) + x] = depth;
}

void context_write_output(const context *c, int x, int y, colour4 colour) {

    c->out_buffer->colour_buffer[(c->vp_width * y) + x] = colour;
}

void context_output_image_ppm(context *c, const char *file_name) {

    FILE* img = fopen(file_name, "w");

    if (!img) {
        printf("Context failed to open file: %s for ppm image output", file_name);
        return;
    }

    fprintf(img, "P3 \n%i %i \n255\n", c->vp_width, c->vp_height);

    for (int y = 0; y < c->vp_height; ++y) {
        for (int x = 0; x < c->vp_width; ++x) {

            float r_f = c->out_buffer->colour_buffer[(c->vp_width * y) + x].e[0];
            float g_f = c->out_buffer->colour_buffer[(c->vp_width * y) + x].e[1];
            float b_f = c->out_buffer->colour_buffer[(c->vp_width * y) + x].e[2];

            r_f = r_f > 1.0f ? 1.0f : r_f < 0.0f ? 0.0f : r_f;
            g_f = g_f > 1.0f ? 1.0f : g_f < 0.0f ? 0.0f : g_f;
            b_f = b_f > 1.0f ? 1.0f : b_f < 0.0f ? 0.0f : b_f;

            int r = (int)(r_f * 255.99f);
            int g = (int)(g_f * 255.99f);
            int b = (int)(b_f * 255.99f);

            fprintf(img, "%i %i %i\n", r, g, b);
        }
    }

    fclose(img);
}

void context_cleanup(context *c) {

    for (size_t i = 0; i < MAX_UNIFORMS; ++i) {
        if (c->uniform_buffer[i].value_type != UTYPE_NULL || c->uniform_buffer[i].value != NULL) {
            free(c->uniform_buffer[i].value);
        }
    }

    *c = (context){0};
}
