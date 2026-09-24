#ifndef LIGHT_H
#define LIGHT_H
#include "vec3.h"

typedef struct {

    vec3 direction;
    float intensity;
    colour3 colour;
}directional_light;

typedef struct {

    vec3 position;
    float intensity;
    colour3 colour;

    struct {
        float radius;

        float constant;
        float linear;
        float quadratic;

    }   attenuation;


} point_light;


// Derive the linear and quadratic coefficients of a point light from a given radius, threshold and constant coeff
void p_light_atten_from_radius(point_light* light, float radius);

// Directly set point light coefficients
void p_light_set_atten_coefficients(point_light* light, float constant, float linear, float quadratic);
#endif