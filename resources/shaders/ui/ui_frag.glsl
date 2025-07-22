#version 310 es
precision mediump float;
precision mediump int;

uniform sampler2D tex;

in vec2 coord;
out vec4 out_color;

void main() {
    vec4 color = texture(tex, coord);
    if(color.a < 0.5f) {
        discard;
    }
    out_color = color;
}