cmake_minimum_required(VERSION 4.1)

project(Rasterizer LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS  ON)

set(LOCAL_DEPENDENCIES
        ${PROJECT_SOURCE_DIR}/local_dependencies
)

set(STB_DIR
        ${LOCAL_DEPENDENCIES}/stb
)

set(FAST_OBJ_DIR
        ${LOCAL_DEPENDENCIES}/fast_obj
)

set(INCLUDE_DIRS
        ${PROJECT_SOURCE_DIR}
        ${PROJECT_SOURCE_DIR}/src
        ${PROJECT_SOURCE_DIR}/src/math
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/include/math

        ${LOCAL_DEPENDENCIES}
        ${STB_DIR}
        ${FAST_OBJ_DIR}
)

set(PROJECT_SOURCES
        src/main.c
        src/context.c
        src/shader_value.c
        src/renderer.c
        src/polygon.c
        src/shader.c
        src/framebuffer.c
        src/light.c
        src/light.c
        src/material.c
        src/texture.c

        src/math/vec2.c
        src/math/vec3.c
        src/math/vec4.c
        src/math/mat3.c
        src/math/mat4.c
        src/math/rasmath.c
        src/math/common_math.c

        ${STB_DIR}/stb.c
        ${FAST_OBJ_DIR}/fast_obj.c
)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIRS})

if(UNIX AND NOT APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE m)
endif()