#ifndef MATERIAL_H
#define MATERIAL_H

#include "math/vec3.h"

typedef struct {

    vec3 color;
    float ambient;
    float diffuse;
    float specular;
    float shininess;
} material;

void material_initialize(material* m);
#endif