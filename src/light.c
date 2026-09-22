#include "light.h"


void p_light_atten_from_radius(point_light *light, float radius) {

    float THRESHOLD = 0.01f;
    float RATIO = 1.0f;
    float CONSTANT = 1.0f;

    float req_quotient = 1.0f / THRESHOLD - 1.0f;

    float quadratic = req_quotient / ((RATIO + 1.0f) * radius * radius);
    float linear = RATIO * quadratic * radius;

    light->attenuation.constant = CONSTANT;
    light->attenuation.linear = linear;
    light->attenuation.quadratic = quadratic;

    light->attenuation.radius = radius;

}

void p_light_set_atten_coefficients(point_light *light, float constant, float linear, float quadratic) {

    light->attenuation.constant = constant;
    light->attenuation.linear = linear;
    light->attenuation.quadratic = quadratic;


}
