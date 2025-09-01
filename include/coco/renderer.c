#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <coco/file.c>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include <cwobj.h>

typedef struct {
    int vbo;
    int size;
} Buffer;

typedef struct {
    int vao;
    int vertexCount;
} Mesh;

typedef struct {
    int vertexShader;
    int fragmentShader;
    int program;
} Shader;

typedef struct {
    int texture;
    int width;
    int height;
    int channels;
    char* data;
} Texture;

typedef struct {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} Color;

Texture* createTexture(char* path, bool aliased) {
    Texture* texture = malloc(sizeof(Texture));
    texture->data = stbi_load(path, &texture->width, &texture->height, &texture->channels, 0);

    glGenTextures(1, &texture->texture);
    glBindTexture(GL_TEXTURE_2D, texture->texture);
    glTexImage2D(GL_TEXTURE_2D, 0, texture->channels == 4 ? GL_RGBA : GL_RGB, texture->width, texture->height,
                 0, texture->channels == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, texture->data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    if (aliased) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    } else {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }

    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    return texture;
}

void saveTexture(char* path, Texture* texture) {
    stbi_write_png(path, texture->width, texture->height, texture->channels, texture->data, 0);
}

void updateTexture(Texture* texture, bool aliased) {
    glDeleteTextures(1, &texture->texture);

    glGenTextures(1, &texture->texture);

    glBindTexture(GL_TEXTURE_2D, texture->texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->width, texture->height, 0, (texture->channels == 4 ? GL_RGBA : GL_RGB), GL_UNSIGNED_BYTE, texture->data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    if(aliased) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    } else {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture* copyTexture(Texture* orig) {
    Texture* dest = malloc(sizeof(Texture));

    dest->width = orig->width;
    dest->height = orig->height;
    dest->channels = orig->channels;

    dest->texture = orig->texture;
    dest->data = malloc(orig->width * orig->height * orig->channels);
    memcpy(dest->data, orig->data, orig->width * orig->height * orig->channels);

    return dest;
}

Color getPixel(Texture* texture, int x, int y) {
    if(x < 0 || x >= texture->width || y < 0 || y >= texture->height) return (Color){0, 0, 0};
    return (Color){
        (unsigned char)texture->data[(x + y * texture->width) * texture->channels],
        (unsigned char)texture->data[(x + y * texture->width) * texture->channels + 1],
        (unsigned char)texture->data[(x + y * texture->width) * texture->channels + 2]
    };
}

void setPixel(Texture* texture, int x, int y, Color color) {
    if(x < 0 || x >= texture->width || y < 0 || y >= texture->height) return;
    texture->data[(x + y * texture->width) * texture->channels] = (char)color.r;
    texture->data[(x + y * texture->width) * texture->channels + 1] = (char)color.g;
    texture->data[(x + y * texture->width) * texture->channels + 2] = (char)color.b;
}

Buffer* createFloatBuffer(float* data, int size) {
    Buffer* buffer = malloc(sizeof(Buffer));

    buffer->size = size;

    glGenBuffers(1, &buffer->vbo);

    glBindBuffer(GL_ARRAY_BUFFER, buffer->vbo);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return buffer;
}

Buffer* createIntBuffer(int* data, int size) {
    Buffer* buffer = malloc(sizeof(Buffer));

    buffer->size = size;

    glGenBuffers(1, &buffer->vbo);

    glBindBuffer(GL_ARRAY_BUFFER, buffer->vbo);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return buffer;
}

Mesh* createMesh(Buffer* vertexBuffer, Buffer* coordBuffer, Buffer* normalBuffer, Buffer* indexBuffer) {
    Mesh* mesh = malloc(sizeof(Mesh));

    mesh->vertexCount = indexBuffer->size / sizeof(int);

    glGenVertexArrays(1, &mesh->vao);
    glBindVertexArray(mesh->vao);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer->vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glBindBuffer(GL_ARRAY_BUFFER, coordBuffer->vbo);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer->vbo);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer->vbo);

    glBindVertexArray(0);

    return mesh;
}

Mesh* createMeshFast(
        float* vertices, int vertices_size, 
        float* coords, int coords_size, 
        float* normals, int normals_size, 
        int* indices, int indices_size) {
    Buffer* vertexBuffer = createFloatBuffer(vertices, vertices_size);
    Buffer* coordBuffer = createFloatBuffer(coords, coords_size);
    Buffer* normalBuffer = createFloatBuffer(normals, normals_size);
    Buffer* indexBuffer = createIntBuffer(indices, indices_size);
    return createMesh(vertexBuffer, coordBuffer, normalBuffer, indexBuffer);
}

Shader* createShader(char* vertexPath, char* fragmentPath) {
    char* vertexShaderSourceDynamic = malloc(65536);
    for(int i = 0; i < 65536; i++) {vertexShaderSourceDynamic[i] = 0;}
    readFile(vertexShaderSourceDynamic, vertexPath);

    char* fragmentShaderSourceDynamic = malloc(65536);
    for(int i = 0; i < 65536; i++) {fragmentShaderSourceDynamic[i] = 0;}
    readFile(fragmentShaderSourceDynamic, fragmentPath);

    const char* vertexShaderSource = vertexShaderSourceDynamic;
    const char* fragmentShaderSource = fragmentShaderSourceDynamic;

    Shader* shader = malloc(sizeof(Shader));

    shader->vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(shader->vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(shader->vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(shader->vertexShader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(shader->vertexShader, 512, NULL, infoLog);
        printf("Vertex shader was not compiled. Info log: \n%s\n", infoLog);
        exit(-1);
    }
    
    shader->fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(shader->fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(shader->fragmentShader);

    glGetShaderiv(shader->fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(shader->fragmentShader, 512, NULL, infoLog);
        printf("Fragment shader was not compiled. Info log: \n%s\n", infoLog);
        exit(-1);
    }

    shader->program = glCreateProgram();

    glAttachShader(shader->program, shader->vertexShader);
    glAttachShader(shader->program, shader->fragmentShader);
    glLinkProgram(shader->program);

    glGetProgramiv(shader->program, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shader->program, 512, NULL, infoLog);
        printf("Shader program was not compiled. Info log: \n%s\n", infoLog);
        exit(-1);
    }

    return shader;
}

Shader* createShaderRaw(char* vertexShaderSourceDynamic, char* fragmentShaderSourceDynamic) {
    const char* vertexShaderSource = vertexShaderSourceDynamic;
    const char* fragmentShaderSource = fragmentShaderSourceDynamic;

    Shader* shader = malloc(sizeof(Shader));

    shader->vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(shader->vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(shader->vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(shader->vertexShader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(shader->vertexShader, 512, NULL, infoLog);
        printf("Vertex shader was not compiled. Info log: \n%s\n", infoLog);
        exit(-1);
    }
    
    shader->fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(shader->fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(shader->fragmentShader);

    glGetShaderiv(shader->fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(shader->fragmentShader, 512, NULL, infoLog);
        printf("Fragment shader was not compiled. Info log: \n%s\n", infoLog);
        exit(-1);
    }

    shader->program = glCreateProgram();

    glAttachShader(shader->program, shader->vertexShader);
    glAttachShader(shader->program, shader->fragmentShader);
    glLinkProgram(shader->program);

    glGetProgramiv(shader->program, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shader->program, 512, NULL, infoLog);
        printf("Shader program was not compiled. Info log: \n%s\n", infoLog);
        exit(-1);
    }

    return shader;
}

void deleteBuffer(Buffer* buffer) {
    glDeleteBuffers(1, &buffer->vbo);
}

void deleteMesh(Mesh* mesh) {
    glDeleteVertexArrays(1, &mesh->vao);
}

void deleteShader(Shader* shader) {
    glDetachShader(shader->program, shader->vertexShader);
    glDetachShader(shader->program, shader->fragmentShader);
    glDeleteShader(shader->vertexShader);
    glDeleteShader(shader->fragmentShader);
    glDeleteProgram(shader->program);
}

void deleteTexture(Texture* texture) {
    glDeleteTextures(1, &texture->texture);
}

void renderMesh(Mesh* mesh, Shader* shader) {
    glBindVertexArray(mesh->vao);
    glUseProgram(shader->program);
    glDrawElements(GL_TRIANGLES, mesh->vertexCount, GL_UNSIGNED_INT, 0);
    glUseProgram(0);
    glBindVertexArray(0);
}

void setUniformFloat(Shader* shader, char* name, float value) {
    glUseProgram(shader->program);
    int location = glGetUniformLocation(shader->program, name);
    glUniform1f(location, value);
    glUseProgram(0);
}

void setUniformVec2(Shader* shader, char* name, float x, float y) {
    glUseProgram(shader->program);
    int location = glGetUniformLocation(shader->program, name);
    glUniform2f(location, x, y);
    glUseProgram(0);
}

void setUniformVec3(Shader* shader, char* name, float x, float y, float z) {
    glUseProgram(shader->program);
    int location = glGetUniformLocation(shader->program, name);
    glUniform3f(location, x, y, z);
    glUseProgram(0);
}

void setUniformVec4(Shader* shader, char* name, float x, float y, float z, float a) {
    glUseProgram(shader->program);
    int location = glGetUniformLocation(shader->program, name);
    glUniform4f(location, x, y, z, a);
    glUseProgram(0);
}

void setUniformInt(Shader* shader, char* name, int value) {
    glUseProgram(shader->program);
    int location = glGetUniformLocation(shader->program, name);
    glUniform1i(location, value);
    glUseProgram(0);
}

void setUniformIVec2(Shader* shader, char* name, int x, int y) {
    glUseProgram(shader->program);
    int location = glGetUniformLocation(shader->program, name);
    glUniform2i(location, x, y);
    glUseProgram(0);
}

void setUniformIVec3(Shader* shader, char* name, int x, int y, int z) {
    glUseProgram(shader->program);
    int location = glGetUniformLocation(shader->program, name);
    glUniform3i(location, x, y, z);
    glUseProgram(0);
}

void setUniformIVec4(Shader* shader, char* name, int x, int y, int z, int a) {
    glUseProgram(shader->program);
    int location = glGetUniformLocation(shader->program, name);
    glUniform4i(location, x, y, z, a);
    glUseProgram(0);
}

void setUniformMat2(Shader* shader, char* name, float* data) {
    glUseProgram(shader->program);
    int location = glGetUniformLocation(shader->program, name);
    glUniformMatrix2fv(location, 1, GL_TRUE, data);
    glUseProgram(0);
}

void setUniformMat3(Shader* shader, char* name, float* data) {
    glUseProgram(shader->program);
    int location = glGetUniformLocation(shader->program, name);
    glUniformMatrix3fv(location, 1, GL_TRUE, data);
    glUseProgram(0);
}

void setUniformMat4(Shader* shader, char* name, float* data) {
    glUseProgram(shader->program);
    int location = glGetUniformLocation(shader->program, name);
    glUniformMatrix4fv(location, 1, GL_TRUE, data);
    glUseProgram(0);
}

void setUniformFloatArray(Shader* shader, char* name, float* data, int length) {
    glUseProgram(shader->program);
    int location = glGetUniformLocation(shader->program, name);
    glUniform1fv(location, length, data);
    glUseProgram(0);
}

void setUniformIntArray(Shader* shader, char* name, int* data, int length) {
    glUseProgram(shader->program);
    int location = glGetUniformLocation(shader->program, name);
    glUniform1iv(location, length, data);
    glUseProgram(0);
}

void setUniformTexture(Shader* shader, char* name, Texture* texture, int num) {
    glActiveTexture(GL_TEXTURE0 + num);
    glBindTexture(GL_TEXTURE_2D, texture->texture);

    glUseProgram(shader->program);
    int location = glGetUniformLocation(shader->program, name);
    glUniform1i(location, num);
    glUseProgram(0);
}

void createMat4Translate(float* mat, float x, float y, float z) {
    float mat2[] = {
        1.0f, 0.0f, 0.0f, x,
        0.0f, 1.0f, 0.0f, y,
        0.0f, 0.0f, 1.0f, z,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    memcpy(mat, mat2, 16 * sizeof(float));
}

void createMat4Rotate(float* mat, float x, float y, float z) {
    float mat2[] = {
        cos(y) * cos(z), sin(x) * sin(y) * cos(z) - cos(x) * sin(z), cos(x) * sin(y) * cos(z) + sin(x) * sin(z), 0.0f,
        cos(y) * sin(z), sin(x) * sin(y) * sin(z) + cos(x) * cos(z), cos(x) * sin(y) * sin(z) - sin(x) * cos(z), 0.0f,
        -sin(y), sin(x) * cos(y), cos(x) * cos(y), 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    memcpy(mat, mat2, 16 * sizeof(float));
}

void createMat4Scale(float* mat, float x, float y, float z) {
    float mat2[] = {
        x,    0.0f, 0.0f, 0.0f,
        0.0f, y,    0.0f, 0.0f,
        0.0f, 0.0f, z,    0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    memcpy(mat, mat2, 16 * sizeof(float));
}

void createMat4Uniform(float* mat) {
    float mat2[] = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    memcpy(mat, mat2, 16 * sizeof(float));
}

void createMat4Ortho(float* mat, float left, float right, float bottom, float top, float near, float far) {
    float mat2[] = {
        1.0f / (right - left) * 2.0f, 0.0f, 0.0f, -left - 1.0f,
        0.0f, 1.0f / (top - bottom) * 2.0f, 0.0f, -bottom - 1.0f,
        0.0f, 0.0f, 1.0f / (far - near), -near,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    memcpy(mat, mat2, 16 * sizeof(float));
}

void createMat4Multiply(float* mat, float* mat0, float* mat1) {
    float mat2[16];
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            mat2[i + j * 4] = mat0[i + 0 * 4] * mat1[0 + j * 4] + mat0[i + 1 * 4] * mat1[1 + j * 4] + mat0[i + 2 * 4] * mat1[2 + j * 4] + mat0[i + 3 * 4] * mat1[3 + j * 4];
        }
    }
    memcpy(mat, mat2, 16 * sizeof(float));
}

Mesh* loadMesh(char* path) {
    cwobj* mesh = cwobj_load(path, NULL);

    float* vertices = mesh->geometry->vertice;
    int vertex_count = mesh->geometry->vertice_n * 3 * sizeof(float);

    float* coords = mesh->geometry->texcoord;
    int coord_count = mesh->geometry->texcoord_n * 2 * sizeof(float);

    float* normals = mesh->geometry->normal;
    int normal_count = mesh->geometry->normal_n * 3 * sizeof(float);

    int* indices = mesh->geometry->indice;
    int index_count = mesh->geometry->indice_n * sizeof(int);

    return createMeshFast(vertices, vertex_count, coords, coord_count, normals, normal_count, indices, index_count);
}