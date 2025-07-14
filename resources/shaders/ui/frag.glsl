#version 460 core

uniform sampler2D tex;

in vec2 coord;
out vec4 out_color;

void main() {
    vec4 color = texture(tex, coord);
    if(color.a < 0.1f) {
        discard;
    }
    out_color = color;
}