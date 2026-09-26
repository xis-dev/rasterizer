# Description
CPU-based 3D software rasterizer written in C11, implementing a programmable rendering pipeline.

<img width="1600" height="900" alt="img_1" src="https://github.com/user-attachments/assets/e9b8ab0d-6c13-4b63-a7df-4b8211dbef9a" />

# Dependencies
- STB(Texture Loading, Locally-Handled): https://github.com/nothings/stb

# Usage

## CMake Configuration
> [!NOTE]
> Replace "../rasterizer" with the relative or absolute directory, where the rasterizer library is placed

```cmake
add_subdirectory(../rasterizer rasterizer)
target_link_libraries(${PROJECT_NAME} PRIVATE rasterizer)

```
## Context Creation & Buffer Binding
Default initialize the context object and viewport size, additionally giving the context the location for the vertex buffer and optionally an index buffer.
```c++
    context ctx;
    context_default_initialize(&ctx);
    context_set_viewport_size(&ctx, WIDTH, HEIGHT);

    ctx.vertex_buffer = vertices;
    ctx.index_buffer = indices;
```

Alongside these the framebuffer must also be created and bound

```c++
    framebuffer basic;
    framebuffer_construct(&basic, WIDTH, HEIGHT);

    ctx.out_buffer = &basic;
```

## Uniform Binding
Setting values in the uniform buffer requires first creating a shader value at the location and setting its value.
```c++
    sv_create(model, mat4,      &ctx.uniform_buffer[UNIFORM_SLOT_1]);
    sv_create(view, mat4,       &ctx.uniform_buffer[UNIFORM_SLOT_2]);
    sv_create(projection, mat4, &ctx.uniform_buffer[UNIFORM_SLOT_3]);

```
The value in a uniform slot can be changed to one of the same type without recreation.
```c++
    sv_change_value(&ctx.uniform_buffer[UNIFORM_SLOT_3], model);
```

If the user wishes to replace a uniform at a location with one of another type, it must first be cleaned.
```c++
    context_clean_uniform_at(&ctx, UNIFORM_SLOT_3);
    sv_create(camera_pos, vec3, &ctx.uniform_buffer[UNIFORM_SLOT_3]);
```
## Shader, Material, Texture & Light Binding
The shader program requires 2 functions, a vertex and fragment function and is bound to the context by copy.

```c++
    ctx.shader = (shader_program){.vertex_shader = &default_vert_shader, .fragment_shader = &blinn_phong_frag_shader};
```

A single material may be default initialized and is bound to the context by copy.
```c++
    material container_mat;
    material_default_initialize(&container_mat);
    
    ctx.material = container_mat;
```

Lights are bound to chosen positions into the context's array for the specific light type.

```c++
    point_light light;
    p_light_default_construct(&light);
    p_light_atten_from_radius(&light, 10.0f);

    ctx.point_lights[0] = light;
```
Textures must be loaded and then bound into the context's array at the chosen position.
```c++
    texture container_tex;
    texture_load_texture(&container_tex, "container.jpg", true);
 
    ctx.textures[TEXTURE_SLOT_1] = &container_tex;
```

## Drawing & Output
Once the context is properly setup, drawing triangles is done either from the index buffer or purely vertex buffer and output to the chosen PPM image.
```c++
    context_clear_colour(&ctx, (vec4){0.0f, 0.0f, 0.0f});
    context_clear_depth(&ctx);
    
    //draw_listed_triangles(&ctx, 24); // Drawing without an index buffer
    draw_indexed_triangles(&ctx, 36);

    context_output_image_ppm(&ctx, "image.ppm");
```

## Cleanup
Once no longer needed, the context can be cleaned, freeing allocated memory for the shader uniforms and unbinding all buffers and values. Loaded textures and created framebuffers must also be freed by the user.

```c++
    // Cleanup context and destroy objects
    context_cleanup(&ctx);
    framebuffer_destroy(&basic);
    texture_destroy(&container_tex);
```
# Architecture
## Rendering Context
Information structure for the current state of the rendering pipeline, vertex, index & output buffers, uniforms, textures, material, lights, etc.
```c++
typedef struct context {

    vertex* vertex_buffer;
    uint32_t* index_buffer;

    shader_value uniform_buffer[MAX_UNIFORMS];

    directional_light dir_lights[MAX_DIRECTIONAL_LIGHTS];
    point_light point_lights[MAX_POINT_LIGHTS];

    shader_program shader;
    material material;

    texture* textures[MAX_TEXTURES];
    framebuffer* out_buffer;

    int vp_width;
    int vp_height;

} context;
```
## Vertices: Attributes & Varyings
Vertices are currently stored as a struct of its attributes and varyings, and must be manually interpolated.

```c++
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
```
## Framebuffer
The framebuffer is currently a single RGBA floating-point format buffer storing colours [0, 1], with a floating-point depth buffer storing reciprocal depth [0, 1].
```c++
typedef struct {
    colour4* colour_buffer;
    float* depth_buffer;

    int width;
    int height;
} framebuffer;
```
## Shader Program
Shader programs are defined as a structure with 2 function pointers representing the fragment and vertex shader.

```c++
typedef struct {

    vertex (* vertex_shader)  (const context*, vertex);
    colour4(* fragment_shader)(const context*, vertex*);
} shader_program;
```
## Uniforms 
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
# Process

### Assembly
Given the current vertex buffer and optionally index buffer, the renderer assembles vertices in batches of 3 as triangles.
```c++
    for (size_t i = 0; (i + 3) <= index_count; i += 3) {
        process_source_triangle(ctx,
                                ctx->vertex_buffer[ctx->index_buffer[i]],
                                ctx->vertex_buffer[ctx->index_buffer[i + 1]],
                                ctx->vertex_buffer[ctx->index_buffer[i + 2]]);
    }
```

### Clipping & Triangulation
Using the Sutherland-Hodgeman polygon clipping algorithm, triangles are clipped against the view frustum's planes, then triangulated as a triangle fan. 
```c++
    polygon in;
    polygon_construct(&in);
    polygon_add_vertex(&in, v1);
    polygon_add_vertex(&in, v2);
    polygon_add_vertex(&in, v3);

    // Test edges against each clip plane
    for (int p = 0; p < 6; ++p) {
        polygon_lazy_clear(&out);

        for (int v = 0; v < in.count; ++v) {
            // Get both vertices that form the edge(clockwise ordered)
            const vertex* a = &in.data[v];
            const vertex* b = &in.data[(v + 1) % in.count];

            const float da = vec4_dot(a->pos, clip_planes[p]);
            const float db = vec4_dot(b->pos, clip_planes[p]);

            if (da >= 0.0f && db >= 0.0f) { // Both vertices are inside, add only second vertex
                polygon_add_vertex(&out, *b);
            }
            else if (da >= 0.0f && db < 0.0f) { // First vertex inside, second outside, add only point of intersection

                const float t = da / (da - db);
                polygon_add_vertex(&out,get_interpolated_vertex(*a, *b, t));

            }
            else if (da < 0.0f && db >= 0.0f) { // First vertex outside, second inside, add point of intersection and second vertex

                const float t = da / (da - db);
                polygon_add_vertex(&out,get_interpolated_vertex(*a, *b, t));

                polygon_add_vertex(&out, *b);
            }

            // Both vertices outside, do nothing
        }
        in = out;
    }
```
### Vertex Shading
The three triangle vertices are processed by the vertex shader and transformed into clip space. After clipping, the vertices undergo perspective division before rasterization.

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
Since perspective projection is not an affine transformation, attributes cannot be interpolated linearly in screen space. Perspective-correct barycentric interpolation is therefore used to recover the appropriate interpolation weights.

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

After barycentric interpolation and we know the pixel we're currently drawing to, the vertex can be passed through the fragment shader, producing a colour, depth testing is then performed after, comparing the vertex's z component with the current depth buffer value.

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


# Sources
- Boreskov, A., & Shikin, E. (2014). Computer Graphics: From pixels to Programmable Graphics Hardware. CRC Press. 
- Brown, R. A. (n.d.). Barycentric coordinates  as  interpolants. Barycentric Coordinates  as  Interpolants. https://arxiv.org/pdf/1308.1279
- GeeksforGeeks. (2024, April 8). Polygon clipping: Sutherland–Hodgman Algorithm. https://www.geeksforgeeks.org/dsa/polygon-clipping-sutherland-hodgman-algorithm/
- Dunn, F., & Parberry, I. (2012). 3D math primer for graphics and game development. CRC Press.
- Oberhollenzer, D. (n.d.). AGENTD/SWRAST: A tiny, just-for-fun software rasterizer written in C. GitHub. https://github.com/AgentD/swrast/tree/master

# Future Considerations

- Mesh loading
- Context flags
- Multiple format framebuffers
- Multiple vertex formats
- Parallelizing rasterization
- Multiple draw primitives
