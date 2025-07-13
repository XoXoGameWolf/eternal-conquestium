#version 460 core

uniform sampler2D tex;
uniform vec2 highlight1;
uniform vec2 highlight2;

in vec2 coord;
out vec4 out_color;

void main() {
    vec4 color = texture(tex, coord);
    if(color.a < 0.1f) {
        discard;
    }
    if(coord.x > highlight1.x && coord.x < highlight2.x && coord.y > highlight1.y && coord.y < highlight2.y) {
        color.xyz += 0.3f;
    }
    out_color = color;
}