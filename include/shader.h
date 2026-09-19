#ifndef SHADER_H
#define SHADER_H

#include "context.h"
#include "vertex.h"
#include "vec4.h"

typedef struct {

    vertex (* vertex_shader)  (const context*, vertex);
    colour4(* fragment_shader)(const context*);
} shader_program;

vertex default_vert_shader(const context* ctx, vertex v);
#endif