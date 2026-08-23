#include <stdbool.h>

/**
 * Structure of elements for a floating point 3D Vector
 * trailing '_mut' signifies mutating the first vector passed in
 * '_sep' signifies a vector is required but the expected arguments are seperated into its individual x,y, z components
*/
typedef struct vec3 {

    double x;
    double y;
    double z;

} vec3;

// Initialize vector with the same scalar value in all elements
vec3 vec3_construct(double s);
vec3 vec3_construct_sep(double x, double y, double z);
void vec3_construct_mut(vec3* v, double s);
void vec3_construct_sep_mut(vec3* v, double x, double y, double z);

double vec3_magnitude(vec3 v1);

vec3 vec3_zero();

vec3 vec3_add(vec3 v1, vec3 v2);
vec3 vec3_add_sep(vec3 v1, double v2_x, double v2_y, double v2_z);
void vec3_add_mut(vec3* v1, vec3 v2);
void vec3_add_sep_mut(vec3* v1, double v2_x, double v2_y, double v2_z);

vec3 vec3_sub(vec3 v1, vec3 v2);
vec3 vec3_sub_sep(vec3 v1, double v2_x, double v2_y, double v2_z);
void vec3_sub_mut(vec3* v1, vec3 v2);
void vec3_sub_sep_mut(vec3* v1, double v2_x, double v2_y, double v2_z);

vec3 vec3_scale(vec3 v1, float s);
void vec3_scale_mut(vec3* v1, float s);

// mutl & div define multiplication and division for each element of a vector
vec3 vec3_mult(vec3 v1, vec3 v2);
void vec3_mult_mut(vec3* v1, vec3 v2);
vec3 vec3_mult_sep(vec3 v1, double v2_x, double v2_y, double v2_z);
void vec3_mult_sep_mut(vec3* v, double v2_x, double v2_y, double v2_z);

vec3 vec3_div(vec3 v1, vec3 v2);
void vec3_div_mut(vec3* v1, vec3 v2);
vec3 vec3_div_sep(vec3 v1, double v2_x, double v2_y, double v2_z);
void vec3_div_sep_mult(vec3* v, double v2_x, double v2_y, double v2_z);

float vec3_dot(vec3 v1, vec3 v2);
float vec3_dot_sep(vec3 v1, double v2_x, double v2_y, double v2_z);
vec3 vec3_cross(vec3 v1, vec3 v2);
vec3 vec3_cross_sep(vec3 v1, double v2_x, double v2_y, double v2_z);

void normalize_mut(vec3* v);
vec3 vec3_get_normalized(vec3 v);

double vec3_dist(vec3 v1, vec3 v2);
double vec3_dist_sq(vec3 v1, vec3 v2);

// Conditional Functions
bool vec3_equals(vec3 v1, vec3 v2);
