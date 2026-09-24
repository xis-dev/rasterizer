#ifndef VERTEX_H
#define VERTEX_H

#include "rasmath.h"
// Base vertex with its attributes
typedef struct {
    vec4 pos;
    vec3 colour;
    vec3 normal;
    vec2 uv_0;
    vec2 uv_1;

    struct {
        vec3 world_pos;
    } varyings;
} vertex;

// Pipeline vertex to travel through rendering pipeline
typedef struct {
    vec4 p_pos;
    vec3 normal;
    vec2 uv_0;
    vec2 uv_1;
} pipeline_vertex;

#endif