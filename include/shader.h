#ifndef SHADER_H
#define SHADER_H

#include "vertex.h"
#include "vec4.h"

typedef struct context context;

typedef struct {

    vertex (* vertex_shader)  (const context*, vertex);
    colour4(* fragment_shader)(const context*, vertex*);
} shader_program;

vertex default_vert_shader(const context* ctx, vertex v);
colour4 blinn_phong_frag_shader(const context* ctx, vertex* v);
#endif