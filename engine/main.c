#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "../include/cglm/cglm.h"

#include "../include/glad.h"
#include <GLFW/glfw3.h>

#include "../game/resources.h"


#include "models.h"

#include "platform.h"
#include "textures.h"


int SCR_WIDTH = 800, SCR_HEIGHT = 600;

int g_width=800, g_height=600;

const char* fragsource = (const char*)shaders_fragment_shader_glsl;
const char* vertsource = (const char*)shaders_vertex_shader_glsl;

extern unsigned char shaders_vat_shader_glsl[];
extern unsigned int shaders_vat_shader_glsl_len;
const char* vatsource = (const char*)shaders_vat_shader_glsl;

int main() {
    GLFWwindow* window = engine_create_window("engine", SCR_WIDTH, SCR_HEIGHT);

    toggle_fullscreen(window, &SCR_WIDTH, &SCR_HEIGHT, 1);

    GLuint vertexShader = compile_shader(GL_VERTEX_SHADER, vertsource, "vertexShader");

    GLuint fragmentShader = compile_shader(GL_FRAGMENT_SHADER, fragsource, "fragmentShader");

    GLuint shaderProgram = create_shader_program(vertexShader, fragmentShader);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    mat4 model = GLM_MAT4_IDENTITY_INIT;
    mat4 view = GLM_MAT4_IDENTITY_INIT;
    mat4 projection = GLM_MAT4_IDENTITY_INIT;

    glm_perspective(glm_rad(90.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f, projection);

    vec3 front = {0.0f,0.0f,0.0f};

    vec3 cameraPos = {0.0f, 1000.0f, 3.0f};
    vec3 cameraTarget = {0.0f, 0.0f, 0.0f};
    vec3 cameraUp = {0.0f, 1.0f, 0.0f};
    glm_lookat(cameraPos, cameraTarget, cameraUp, view);

    GLuint tex_terrain = load_texture(textures_grass_raw, textures_grass_raw_len, 1960, 1960, 4);

    GLuint tex_coin = load_texture(textures_coin_raw, textures_coin_raw_len, 3508, 2480, 4);


    mesh terrain = create_mesh(model_Terrain_vertices, MODEL_Terrain_VERTEX_COUNT, model_Terrain_indices, MODEL_Terrain_INDEX_COUNT);
    terrain.texture = tex_terrain;

    mesh coin = create_mesh(model_coin_vertices, MODEL_coin_VERTEX_COUNT, model_coin_indices, MODEL_coin_INDEX_COUNT);
    coin.texture = tex_coin;

    double previousTime = glfwGetTime();
    int fps = 0;


    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    glEnable(GL_CULL_FACE);

    glEnable(GL_DEPTH_TEST);



    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (glfwRawMouseMotionSupported())
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

    GLFWcursor* url_cursor = glfwCreateStandardCursor(GLFW_POINTING_HAND_CURSOR);

    double oldposx, oldposy;
    double xpos, ypos;

    float sensivity = 0.1f;
    vec3 right;

    vec3 forward;

    float yaw = -90.0f;
    float pitch = 0.0f;

    float falling_speed = 0.0f;
    bool first_jump;

    while (!glfwWindowShouldClose(window)) {

        float speed = 15.0f;

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

        float yaw_rad = yaw * M_PI / 180.0f;
        float pitch_rad = pitch * M_PI / 180.0f;

        front[0] = cos(yaw_rad) * cos(pitch_rad);
        front[1] = sin(pitch_rad);
        front[2] = sin(yaw_rad) * cos(pitch_rad);

        vec3 horizontal_front;
        glm_vec3_copy(front, horizontal_front);
        horizontal_front[1] = 0.0f;
        glm_vec3_normalize(horizontal_front);

        glm_vec3_cross(front, cameraUp, right);
        glm_vec3_normalize(right);

        vec3 target;
        glm_vec3_add(cameraPos, front, target);
        glm_lookat(cameraPos, target, cameraUp, view);


        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, 1);
        }

        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
            speed *= 4;
        }

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            cameraPos[0] += horizontal_front[0] * speed * deltaTime;
            cameraPos[1] += horizontal_front[1] * speed * deltaTime;
            cameraPos[2] += horizontal_front[2] * speed * deltaTime;
        }

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            cameraPos[0] += right[0] * speed * deltaTime * -1.0f;
            // cameraPos[1] += right[1] * speed * deltaTime * -1.0f;
            cameraPos[2] += right[2] * speed * deltaTime * -1.0f;
        }

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            cameraPos[0] += horizontal_front[0] * speed * deltaTime * -1.0f;
            // cameraPos[1] += front[1] * speed * deltaTime * -1.0f;
            cameraPos[2] += horizontal_front[2] * speed * deltaTime * -1.0f;
        }

        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            cameraPos[0] += right[0] * speed * deltaTime;
            // cameraPos[1] += right[1] * speed * deltaTime;
            cameraPos[2] += right[2] * speed * deltaTime;
        }

        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            if(cameraPos[1] <= 3 && first_jump == true) falling_speed -= 1;
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
            falling_speed += 1;
        }

        if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
            toggle_fullscreen(window, &g_width, &g_height, 0);
            glm_perspective(glm_rad(90.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f, projection);

        }

        if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
            toggle_fullscreen(window, &g_width, &g_height, 1);

        }

        if(cameraPos[1] > 3) first_jump = false;
        if(cameraPos[1] > 2) first_jump = true;

        float currentFrame = (float)glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;


        if(cameraPos[1] > 2.0) falling_speed += 9.8*deltaTime;
        else if(falling_speed > 0) falling_speed = 0;

        cameraPos[1] -= falling_speed * 10 * deltaTime;

        glClearColor(0.11f, 0.145f, 0.29f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glfwGetFramebufferSize(window, &SCR_WIDTH, &SCR_HEIGHT);
        float aspect = (float)SCR_WIDTH / (float)SCR_HEIGHT;
        glm_perspective(glm_rad(90.0f), aspect, 0.1f, 1000.0f, projection);

        glUseProgram(shaderProgram);

        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, model[0]);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, view[0]);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, projection[0]);

        renderMesh(terrain);

        renderMesh(coin);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    engine_destroy_window(window);

    return 0;
}
