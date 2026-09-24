# Description
Software rasterizer written in C to understand the rendering process. Outputs to a PPM file.

# Process

### Assembly
Given the current vertex buffer and optionally index buffer, the renderer assembles vertices in batches of 3 as triangles.

### Vertex Shading
The three triangle vertices go through vertex shading, projection and the perspective divide after.

```c++
    mat4 model_matrix;
    mat4 mvp_matrix;

    sv_get_value(ctx->uniform_buffer[UNIFORM_SLOT_1], &model_matrix);
    sv_get_value(ctx->uniform_buffer[UNIFORM_SLOT_5], &mvp_matrix);

    v.varyings.world_pos = vec3_convert_vec4(mat4_transform(model_matrix, v.pos));
    v.pos =  mat4_transform(mvp_matrix, v.pos);
    v.normal = mat3_transform(
            mat3_get_transpose(mat3_get_inverse(mat3_convert_mat4(model_matrix))),
               v.normal);

    return v;
```

### Rasterization
Given the clip space vertices after projection, we can convert to screen space for rasterization.

Using only pixels contained within the bounding box of the triangle, we can evaluate the edge function of the pixel for each edge, if all the edge functions provide the expected signage, the point must lie inside the triangle.
```c++
    // Edge function for a set of clockwise vertices returning positive if on the right side of plane
    static float edge_function(vec2 a, vec2 b, vec2 c) {
        return (c.e[0] - a.e[0]) * (b.e[1] - a.e[1]) - (c.e[1] - a.e[1]) * (b.e[0] - a.e[0]);
    }
    
```

```c++
    for (int y = yMin; y <= yMax; ++y) {

        for (int x = xMin; x <= xMax; ++x) {

            const vec2 p = (vec2){(float)x, (float)y};

            const vec2 v1_2D = vec2_convert_vec4(v1.pos);
            const vec2 v2_2D = vec2_convert_vec4(v2.pos);
            const vec2 v3_2D = vec2_convert_vec4(v3.pos);

            const float e1 = edge_function(v2_2D, v3_2D, p);
            const float e2 = edge_function(v3_2D, v1_2D, p);
            const float e3 = edge_function(v1_2D, v2_2D, p);


            // if all edge functions fall within the positive plane, the point must be in the triangle
            if (e1 >= 0 && e2 >= 0 && e3 >= 0) {
                // Perform rasterization
        }
```

### Barycentric Interpolation
Due to the vertex positions being stored in screen space, whilst our other attributes and varyings are in a space we cannot access from a linear transformation.

The barycentric interpolants are derived using the area of the parallelogram formed by 2 triangle edges, undoing the projection process and obtaining barycentric coordinates for interpolation in view space.

```c++
    // Compute barycentric using ratio between parallelograms formed by main and inner triangles
    vec3 b = compute_barycentric(v1.pos, e1, v2.pos, e2, v3.pos, e3);

    // Find areas in camera/eye space
    float area_1_eye = v2.e[3] * v3.e[3] * area_1;
    float area_2_eye = v3.e[3] * v1.e[3] * area_2;
    float area_3_eye = v1.e[3] * v2.e[3] * area_3;
    float total_area = area_1_eye + area_2_eye + area_3_eye;

    vec3 b_out;
    b_out.e[0] = area_1_eye / total_area;
    b_out.e[1] = area_2_eye / total_area;
    b_out.e[2] = area_3_eye / total_area;

    return b_out;
```

### Fragment Shading & Depth Testing

After barycentric interpolation and we know the pixel we're currently drawing to, the vertex can be passed through the vertex shader, producing a colour, depth testing is then performed after, comparing the vertex's z component with the current depth buffer value.

```c++

    vertex fragment_vert = get_bary_interpolated_vertex(v1, v2, v3, b);

    fragment_vert.pos.e[0] = (float)x;
    fragment_vert.pos.e[1] = (float)y;

    colour4 frag_colour = ctx->shader.fragment_shader(ctx, &fragment_vert);

```
### Depth Testing and Output
After the fragment shader is run, we can depth test against the current depth value in the buffer, on success, the fragment colour is written to the colour buffer.
```c++
    if (depth_test(ctx, x, y, fragment_vert.pos.e[2])) {
        context_write_output(ctx, x, y, frag_colour);
    }

```

# Architecture
## Rendering Context
Information structure 
## Uniform System
The rasterizer uses type-agnostic uniform values with a set of supported value types, using generic macro selection, we can determine the C type of the variable.

```c++
   typedef enum {UTYPE_NULL, UTYPE_INT32, UTYPE_UINT32, UTYPE_F32, UTYPE_VEC2_F32, UTYPE_VEC3_F32, UTYPE_VEC4_F32, UTYPE_MAT4_F32} u_type;

    typedef struct {
        void* value;
        u_type value_type;
    } shader_value;
    
    
    #define sv_get_value(sv, out_ptr) \
        do { \
        if ((sv).value_type != CTYPE_TO_UTYPE(*(out_ptr))) {\
            printf("SHADER_VALUE: Found mismatched types beteween provided output type and stored shader value type, while trying to get shader value"); \
            break; \
            }\
        memcpy((out_ptr), (sv).value, sizeof(*(out_ptr))); \
    } while (0)
```

# Sources
- Brown, R. A. (n.d.). Barycentric coordinates  as  interpolants. Barycentric Coordinates  as  Interpolants. https://arxiv.org/pdf/1308.1279
- 
# Future Considerations

- Parallelizing rasterization
- Multiple draw configurations