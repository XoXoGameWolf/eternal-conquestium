#version 310 es
precision mediump float;
precision mediump int;

uniform sampler2D tex;
uniform vec3 color;
uniform int data[32];
uniform int length;

in vec2 coord;
out vec4 out_color;

void main() {
    int character = int(coord.x * float(length));
    vec4 _color = texture(tex, vec2(
        coord.x / 256.0f * float(length) + float(data[character]) / 256.0f - float(character) / 256.0f, 
        coord.y
    ));

    if(_color.r < 0.5f) {
        discard;
    }

    _color.rgb *= color;

    out_color = _color;
}