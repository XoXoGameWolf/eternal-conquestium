#version 460 core

in vec3 in_pos;
in vec2 in_coord;
out vec2 coord;

void main() {
    coord = in_coord;
    gl_Position = vec4(in_pos * 2, 1.0f);
}