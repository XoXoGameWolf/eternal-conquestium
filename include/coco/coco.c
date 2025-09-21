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

void framebuffer_size_callback(GLFWwindow* window, int _width, int _height);

#include <coco/renderer.c>
#include <coco/objects.c>
#include <coco/input.c>
#include <coco/audio.c>

void framebuffer_size_callback(GLFWwindow* window, int _width, int _height) {
    for(int i = 0; i < 256; i++) {
        if(viewports[i] == 0) continue;
        glBindFramebuffer(GL_FRAMEBUFFER, viewports[i]->fbo);
        glViewport(0, 0, _width, _height);
        
        deleteTexture(viewports[i]->texture);
        deleteTexture(viewports[i]->depth);
        deleteTexture(viewports[i]->texture2);

        viewports[i]->texture = createEmptyTexture(_width, _height, false);
        viewports[i]->depth = createEmptyDepthTexture(_width, _height, false);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, viewports[i]->texture->texture, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, viewports[i]->depth->texture, 0);

        viewports[i]->texture2->width = _width;
        viewports[i]->texture2->height = _height;
        viewports[i]->texture2->texture = createEmptyTexture(_width, _height, false)->texture;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, _width, _height);
}

void error_callback(int id, const char* description) {
    // ignore "Invalid window size 0x0" error
    if(strlen(description) >= 23 && description[20] == '0' && description[21] == 'x' && description[22] == '0') {
        return;
    }
    printf("GLFW had an error: \n%s\n", description);
}

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

    GLFWimage images[1];
    images[0].pixels = stbi_load("resources/icon.png", &images[0].width, &images[0].height, 0, 4);
    glfwSetWindowIcon(window, 1, images);
    stbi_image_free(images[0].pixels);

    audioDevice = alcOpenDevice(NULL);

    audioContext = alcCreateContext(audioDevice, NULL);
    alcMakeContextCurrent(audioContext);

    alListener3f(AL_POSITION, cam_pos_x, cam_pos_y, cam_pos_z);

    for(int i = 0; i < 256; i++) {
        audios[i] = 0;
        audioSources[i] = 0;
        buffers[i] = 0;
        meshes[i] = 0;
        shaders[i] = 0;
        textures[i] = 0;
    }

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

        alListener3f(AL_POSITION, cam_pos_x, cam_pos_y, cam_pos_z);

        for(int i = 0; i < 256; i++) {
            if(audioSources[i] != 0 && !getAudioSourceState(audioSources[i])) {
                deleteAudioSource(audioSources[i]);
            }

            if(audioSources[i] != 0 && audioSources[i]->camera) {
                setAudioSourcePos(audioSources[i], cam_pos_x, cam_pos_y, cam_pos_z);
            }

            if(audioSources[i] != 0 && audioSources[i]->object != 0) {
                setAudioSourcePos(audioSources[i], 
                    audioSources[i]->object->pos_x, 
                    audioSources[i]->object->pos_y, 
                    audioSources[i]->object->pos_z
                );
            }
        }

        glfwSwapBuffers(window);
        glfwPollEvents();

        int width2;
        int height2;

        glfwGetWindowSize(window, &width2, &height2);

        if(width2 != width || height2 != height) {
            width = width2;
            height = height2;

            for(int i = 0; i < 256; i++) {
                if(viewports[i] == 0) continue;
                glBindFramebuffer(GL_FRAMEBUFFER, viewports[i]->fbo);
                
                deleteTexture(viewports[i]->texture);
                deleteTexture(viewports[i]->depth);
                deleteTexture(viewports[i]->texture2);

                viewports[i]->texture = createEmptyTexture(width, height, false);
                viewports[i]->depth = createEmptyDepthTexture(width, height, false);

                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, viewports[i]->texture->texture, 0);
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, viewports[i]->depth->texture, 0);

                viewports[i]->texture2->width = width;
                viewports[i]->texture2->height = height;
                viewports[i]->texture2->texture = createEmptyTexture(width, height, false)->texture;
            }

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

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

    for(int i = 0; i < 256; i++) {
        if(audios[i] != 0) {
            deleteAudio(audios[i]);
        }
        if(audioSources[i] != 0) {
            deleteAudioSource(audioSources[i]);
        }
        if(buffers[i] != 0) {
            deleteBuffer(buffers[i]);
        }
        if(meshes[i] != 0) {
            deleteMesh(meshes[i]);
        }
        if(shaders[i] != 0) {
            deleteShader(shaders[i]);
        }
        if(textures[i] != 0) {
            deleteTexture(textures[i]);
        }
    }

    alcDestroyContext(audioContext);
    alcCloseDevice(audioDevice);

    glfwTerminate();
}