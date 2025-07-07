#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

GLFWwindow* window_window;

int window_width;
int window_height;

bool window_lastFullscreen;
bool window_fullscreen;
bool window_open;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void error_callback(int id, const char* description) {
    printf("GLFW had an error: \n%s\n", description);
}

void window_init(char* title, int width, int height) {
    window_width = width;
    window_height = height;

    window_lastFullscreen = false;
    window_fullscreen = false;
    window_open = true;

    glfwSetErrorCallback(&error_callback);

    if(!glfwInit()) {
        printf("GLFW was not initialized.\n");
        exit(-1);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    window_window = glfwCreateWindow(width, height, title, NULL, NULL);
    if(!window_window) {
        printf("Window was not initialized.\n");
        exit(-1);
    }

    glfwMakeContextCurrent(window_window);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("GLAD was not initialized.\n");
        exit(-1);
    }

    float widthScale;
    float heightScale;

    glfwGetWindowContentScale(window_window, &widthScale, &heightScale);

    framebuffer_size_callback(window_window, (int)((float)width * widthScale), (int)((float)height * heightScale));
    glfwSetFramebufferSizeCallback(window_window, framebuffer_size_callback);
}

void window_update() {
    bool lastF11 = glfwGetKey(window_window, GLFW_KEY_F11);

    glfwSwapBuffers(window_window);
    glfwPollEvents();

    glfwGetWindowSize(window_window, &window_width, &window_height);

    if(glfwGetKey(window_window, GLFW_KEY_F11) && !lastF11) {
        window_fullscreen = !window_fullscreen;
    }

    if(window_fullscreen && !window_lastFullscreen) {
        glfwSetWindowMonitor(window_window, glfwGetPrimaryMonitor(), 0, 0, window_width, window_height, GLFW_DONT_CARE);

    } else {
        glfwSetWindowMonitor(window_window, NULL, 0, 0, window_width, window_height, GLFW_DONT_CARE);
    }

    if(glfwWindowShouldClose(window_window)) {
        window_open = false;
    }
}

void window_destroy() {
    glfwTerminate();
}