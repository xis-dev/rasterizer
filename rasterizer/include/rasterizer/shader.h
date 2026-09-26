#ifndef SHADER_H
#define SHADER_H

#include "vertex.h"
#include "math/vec4.h"

typedef struct context context;

typedef struct {

    vertex (* vertex_shader)  (const context*, vertex);
    colour4(* fragment_shader)(const context*, vertex*);
} shader_program;


#endif