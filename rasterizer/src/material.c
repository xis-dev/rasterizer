#include "../include/rasterizer/material.h"

void material_default_initialize(material *m) {

    *m = (material){.color = vec3_construct(1.0f), .ambient = 0.0f, .diffuse = 1.0f, .specular = 1.0f, .shininess = 32.0f};
}
