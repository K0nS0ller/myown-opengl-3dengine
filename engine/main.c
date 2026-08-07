#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "../include/cglm/cglm.h"

#include "../include/glad.h"
#include <GLFW/glfw3.h>


//contents

// shaders
#include "../content/shaders/vertex_shader.h"
#include "../content/shaders/fragment_shader.h"


//imgs
#include "../content/textures/container.h"

//models
#include "../content/assets/model.h"

extern const float model_vertices[];
extern const unsigned int model_indices[];

extern unsigned char textures_container_raw[];
extern unsigned int textures_container_raw_len;


unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 800;

int g_width, g_height;
int need_update_projection = 0;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    g_width = width;
    g_height = height;
    glViewport(0, 0, width, height);
    need_update_projection = 1;
}

extern unsigned char shaders_fragment_shader_glsl[];
extern unsigned int shaders_fragment_shader_glsl_len;

const char *fragsource = (const char *)shaders_fragment_shader_glsl;
const char *vertsource = (const char *)shaders_vertex_shader_glsl;


int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();

    const GLFWvidmode* mode = glfwGetVideoMode(monitor);

    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

    GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, "engine", monitor, NULL);

    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("failed to load GLAD\n");
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    glfwGetFramebufferSize(window, &SCR_WIDTH, &SCR_HEIGHT);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertsource, NULL);
    glCompileShader(vertexShader);
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        printf("Vertex shader error:\n%s\n", infoLog);
    }

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragsource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        printf("fragment shader error:\n%s\n", infoLog);
    }

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        printf("program link error:\n%s\n", infoLog);
        return 0;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


    mat4 model = GLM_MAT4_IDENTITY_INIT;
    mat4 view = GLM_MAT4_IDENTITY_INIT;
    mat4 projection = GLM_MAT4_IDENTITY_INIT;

    glm_perspective(glm_rad(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f, projection);

    vec3 front = {0.0f,0.0f,0.0f};

    vec3 cameraPos = {0.0f, 0.0f, 3.0f};
    vec3 cameraTarget = {0.0f, 0.0f, 0.0f};
    vec3 cameraUp = {0.0f, 1.0f, 0.0f};
    glm_lookat(cameraPos, cameraTarget, cameraUp, view);


    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, MODEL_VERTEX_COUNT * 8 * sizeof(float), model_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, MODEL_INDEX_COUNT * sizeof(unsigned int), model_indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 512, 512 , 0, GL_RGBA, GL_UNSIGNED_BYTE, textures_container_raw);
    glGenerateMipmap(GL_TEXTURE_2D);

    double previousTime = glfwGetTime();
    int fps = 0;


    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    // glEnable(GL_CULL_FACE);

    glEnable(GL_DEPTH_TEST);



    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (glfwRawMouseMotionSupported())
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

    GLFWcursor* url_cursor = glfwCreateStandardCursor(GLFW_POINTING_HAND_CURSOR);

    double oldposx, oldposy;
    double xpos, ypos;

    float sensivity = 0.1f;
    vec3 right;

    float yaw = -90.0f;
    float pitch = 0.0f;

    while (!glfwWindowShouldClose(window)) {

        float speed = 2.5f;

        double currentTime = glfwGetTime();
        fps++;
        if (currentTime - previousTime >= 1.0)
        {
            printf("%d fps\n", fps);
            fps = 0;
            previousTime = currentTime;
        }

        oldposx = xpos;
        oldposy = ypos;

        glfwGetCursorPos(window, &xpos, &ypos);

        float xoffset, yoffset;

        xoffset = xpos-oldposx;
        yoffset = oldposy-ypos;

        xoffset *= sensivity;
        yoffset *= sensivity;

        yaw += xoffset; pitch += yoffset;

        if (pitch > 89.0f) pitch = 89.0f;
        if (pitch < -89.0f) pitch = -89.0f;

        float yaw_rad = yaw * 3.14159f / 180.0f;
        float pitch_rad = pitch * 3.14159f / 180.0f;

        front[0] = cos(yaw_rad) * cos(pitch_rad);
        front[1] = sin(pitch_rad);
        front[2] = sin(yaw_rad) * cos(pitch_rad);

        glm_vec3_cross(front, cameraUp, right);
        glm_vec3_normalize(right);

        vec3 target;
        glm_vec3_add(cameraPos, front, target);
        glm_lookat(cameraPos, target, cameraUp, view);


        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, 1);
        }

        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
            speed *= 2;
        }

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            cameraPos[0] += front[0] * speed * deltaTime;
            cameraPos[1] += front[1] * speed * deltaTime;
            cameraPos[2] += front[2] * speed * deltaTime;
        }

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            cameraPos[0] += right[0] * speed * deltaTime * -1.0f;
            cameraPos[1] += right[1] * speed * deltaTime * -1.0f;
            cameraPos[2] += right[2] * speed * deltaTime * -1.0f;
        }

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            cameraPos[0] += front[0] * speed * deltaTime * -1.0f;
            cameraPos[1] += front[1] * speed * deltaTime * -1.0f;
            cameraPos[2] += front[2] * speed * deltaTime * -1.0f;
        }

        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            cameraPos[0] += right[0] * speed * deltaTime;
            cameraPos[1] += right[1] * speed * deltaTime;
            cameraPos[2] += right[2] * speed * deltaTime;
        }

        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            cameraPos[1] += speed * deltaTime;
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
            cameraPos[1] -= speed * deltaTime;
        }


        float currentFrame = (float)glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glClearColor(0.11f, 0.145f, 0.29f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (need_update_projection) {
            float aspect = (float)g_width / (float)g_height;
            glm_perspective(glm_rad(45.0f), aspect, 0.1f, 100.0f, projection);
            need_update_projection = 0;
        }

        glUseProgram(shaderProgram);

        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, model[0]);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, view[0]);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, projection[0]);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(VAO);

        glDrawElements(GL_TRIANGLES, MODEL_INDEX_COUNT, GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    glDeleteTextures(1, &texture);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
