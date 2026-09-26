cmake_minimum_required(VERSION 4.1)

project(rasterizer LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS  ON)

set(INCLUDE_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include/math
        ${CMAKE_CURRENT_SOURCE_DIR}/external

)

set(EXTERNAL_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/external/stb/stb.c
)
set(RASTERIZER_SOURCES

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

)

add_library(${PROJECT_NAME} STATIC
        ${RASTERIZER_SOURCES}
        ${EXTERNAL_SOURCES}
)

target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIRS})

if(UNIX AND NOT APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE m)
endif()
