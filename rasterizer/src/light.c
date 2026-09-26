#include "../include/rasterizer/light.h"


void p_light_default_construct(point_light *light) {

    *light = (point_light){0};

    light->colour = vec3_construct(1.0f);
    light->intensity = 1.0f;
    p_light_atten_from_radius(light, 50.0f);
    
}

void p_light_atten_from_radius(point_light *light, float radius) {

    float CONSTANT = 1.0f;

    light->attenuation.constant = CONSTANT;
    light->attenuation.linear = 2.0f / radius;
    light->attenuation.quadratic = 1 / (radius * radius);

    light->attenuation.radius = radius;

}

void p_light_set_atten_coefficients(point_light *light, float constant, float linear, float quadratic) {

    light->attenuation.constant = constant;
    light->attenuation.linear = linear;
    light->attenuation.quadratic = quadratic;


}
