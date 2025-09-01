#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

void start();
void update();

GLFWwindow* window;

int width = 1280;
int height = 720;

bool lastFullscreen = false;
bool fullscreen = false;
bool open = true;

void framebuffer_size_callback(GLFWwindow* window, int _width, int _height) {
    glViewport(0, 0, _width, _height);
}

void error_callback(int id, const char* description) {
    // ignore "Invalid window size 0x0" error
    if(strlen(description) >= 23 && description[20] == '0' && description[21] == 'x' && description[22] == '0') {
        return;
    }
    printf("GLFW had an error: \n%s\n", description);
}

#include <coco/renderer.c>
#include <coco/objects.c>
#include <coco/input.c>

Mesh* quad;

int main() {
    glfwSetErrorCallback(&error_callback);

    if(!glfwInit()) {
        printf("GLFW was not initialized.\n");
        exit(-1);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    window = glfwCreateWindow(1280, 720, title, NULL, NULL);
    if(!window) {
        printf("Window was not initialized.\n");
        exit(-1);
    }

    glfwMakeContextCurrent(window);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("GLAD was not initialized.\n");
        exit(-1);
    }

    glEnable(GL_DEPTH_TEST);

    float widthScale;
    float heightScale;

    glfwGetWindowContentScale(window, &widthScale, &heightScale);

    framebuffer_size_callback(window, (int)((float)1280 * widthScale), (int)((float)720 * heightScale));
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    float vertices[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        -1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f
    };

    float coords[] = {
        0.0f, 1.0f,
        1.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f
    };

    float normals[] = {
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f
    };

    int indices[] = {
        0, 1, 3,
        0, 2, 3
    };

    quad = createMeshFast(
        vertices, sizeof(float) * 12, 
        coords, sizeof(float) * 8, 
        normals, sizeof(float) * 12, 
        indices, sizeof(int) * 6
    );

    start();

    while(open) {
        glClearColor(bg_red, bg_green, bg_blue, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        update();
        render();
        bool lastF11 = glfwGetKey(window, GLFW_KEY_F11);

        glfwSwapBuffers(window);
        glfwPollEvents();

        glfwGetWindowSize(window, &width, &height);

        if(glfwGetKey(window, GLFW_KEY_F11) && !lastF11) {
            fullscreen = !fullscreen;
        }

        if(fullscreen && !lastFullscreen) {
            glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), 0, 0, width, height, GLFW_DONT_CARE);

        } else {
            glfwSetWindowMonitor(window, NULL, 0, 0, width, height, GLFW_DONT_CARE);
        }

        if(glfwWindowShouldClose(window)) {
            open = false;
        }
    }

    glfwTerminate();
}