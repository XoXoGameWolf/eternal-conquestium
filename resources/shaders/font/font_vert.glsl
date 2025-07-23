#version 310 es
precision mediump float;
precision mediump int;

uniform float aspect;
uniform int _length;
uniform vec2 pos;
uniform float scale;

in vec3 in_pos;
in vec2 in_coord;
out vec2 coord;

void main() {
    gl_Position = vec4(
        in_pos.x * 2.0f * scale + pos.x, 
        in_pos.y / aspect * 2.0f * scale / float(_length) * 4.0f + pos.y, 
        in_pos.z, 
        1.0f
    );
    coord = in_coord;
}