#pragma once

#include "../include/glad.h"
#include <GLFW/glfw3.h>

GLuint load_texture(const unsigned char* data, size_t len, int width, int height, int channels);
