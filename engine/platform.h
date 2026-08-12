#pragma once

#include "../include/glad.h"
#include <GLFW/glfw3.h>

void toggle_fullscreen(GLFWwindow* window, int* width, int* height, bool fullscreen);

GLFWwindow* engine_create_window(const char* title, int width, int height);

void engine_destroy_window(GLFWwindow* window);

GLuint compile_shader(GLenum type, const char* source, const char* name);

GLuint create_shader_program(GLuint vertexShader, GLuint fragmentShader);

