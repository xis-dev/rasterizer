#include "material.h"

void material_initialize(material *m) {

    *m = (material){.color = vec3_construct(1.0f), .ambient = 1.0f, .diffuse = 1.0f, .specular = 1.0f, .shininess = 1.0f};
}
