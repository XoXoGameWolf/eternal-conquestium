#version 460 core

uniform float aspect;
uniform vec3 camPos;

in vec3 pos;
in vec2 coord;
out vec2 coord_int;

void main() {
    coord_int = coord;
    gl_Position = vec4(
        (2.75f * pos.x - camPos.x) / camPos.z, 
        (pos.y - camPos.y) / camPos.z * aspect, 
        pos.z, 
        1.0f
    );
}