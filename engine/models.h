#pragma once

#include "../include/glad.h"
#include <GLFW/glfw3.h>

typedef struct{
    GLuint VAO, VBO, EBO;
    GLuint texture;
    int vertex_count;
    int index_count;
} mesh;


mesh create_mesh(const float* vertices, int vertex_count, const unsigned int* indices, int index_count);

void renderMesh(mesh mesh);

void destroyMesh(mesh mesh);
