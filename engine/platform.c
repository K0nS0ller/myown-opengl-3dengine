#include <stdio.h>
#include "../include/glad.h"
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdbool.h>

#include "platform.h"


static void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void toggle_fullscreen(GLFWwindow* window, int* width, int* height, bool fullscreen) {
    if (fullscreen) {
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
    } else {
        glfwSetWindowMonitor(window, NULL, 100, 100, *width, *height, 0);
    }
}

GLFWwindow* engine_create_window(const char* title, int width, int height) {


    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return NULL;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWmonitor* monitor = NULL;
    GLFWwindow* window = glfwCreateWindow(width, height, title, monitor, NULL);

    if (!window) {
        fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        return NULL;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        fprintf(stderr, "Failed to load GLAD\n");
        glfwDestroyWindow(window);
        glfwTerminate();
        return NULL;
    }

    int fb_width, fb_height;
    glfwGetFramebufferSize(window, &fb_width, &fb_height);

    glViewport(0, 0, fb_width, fb_height);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    return window;
}


void engine_destroy_window(GLFWwindow* window) {
    glfwDestroyWindow(window);
    glfwTerminate();
}


GLuint compile_shader(GLenum type, const char* source, const char* name) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        fprintf(stderr, "shader compile error. Shader name:%s\nLog:\n%s\n", name, infoLog);
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

GLuint create_shader_program(GLuint vertexShader, GLuint fragmentShader) {
    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        fprintf(stderr, "Program link error:\n%s\n", infoLog);
        glDeleteProgram(program);
        return 0;
    }
    return program;
}

