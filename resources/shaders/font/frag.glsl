#version 460 core

uniform sampler2D tex;
uniform vec3 color;
uniform int data[32];
uniform int length;

in vec2 coord;
out vec4 out_color;

void main() {
    int character = int(coord.x * length);
    vec4 _color = texture(tex, vec2(
        coord.x / 256.0 * length + data[character] / 256.0 - float(character) / 256.0, 
        coord.y
    ));

    if(_color.r == 0) {
        discard;
    }

    _color.rgb *= color;

    out_color = _color;
}