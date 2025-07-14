#version 460 core

uniform float aspect;
uniform int length;
uniform vec2 pos;
uniform float scale;

in vec3 in_pos;
in vec2 in_coord;
out vec2 coord;

void main() {
    gl_Position = vec4(in_pos.x * 2 * scale + pos.x, in_pos.y / aspect * 2 * scale / length * 4 + pos.y, in_pos.z, 1.0f);
    coord = in_coord;
}