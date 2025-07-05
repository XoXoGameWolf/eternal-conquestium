#version 460 core

uniform sampler2D provinceTex;
uniform sampler2D terrainTex;
uniform sampler2D borderTex;
uniform sampler2D colorTex;
uniform vec3 camPos;

in vec2 coord_int;
out vec4 color;

void main() {
    vec4 id = texture(provinceTex, coord_int);

    vec2 coord = vec2((id.b * 255 + 256 * mod(id.g * 255, 16)) / 4096, (id.g * 255 / 16 + 16 * 255 * id.r) / 4096);
    float type = floor(texture(terrainTex, coord).r * 255);

    if(texture(borderTex, coord) != vec4(0.0f, 0.0f, 0.0f, 1.0f) && type != 2) {
        color = texture(colorTex, vec2(texture(borderTex, coord).r, 0));

    } else if(type == 0) {
        color = vec4(1.0f, 1.0f, 1.0f, 1.0f);

    } else if(type == 1) {
        color = vec4(0.13f, 0.45f, 0.05f, 1.0f);

    } else if(type == 2) {
        color = vec4(0.05f, 0.17f, 0.67f, 1.0f);
    }

    if(texture(provinceTex, coord_int + vec2(1.0f / 28160.0f, 0.0f)) != id ||
            texture(provinceTex, coord_int + vec2(0.0f, 1.0f / 10240.0f)) != id ||
            texture(provinceTex, coord_int + vec2(-1.0f / 28160.0f, 0.0f)) != id ||
            texture(provinceTex, coord_int + vec2(0.0f, -1.0f / 10240.0f)) != id ||
            texture(provinceTex, coord_int + vec2(-1.0f / 28160.0f, -1.0f / 10240.0f)) != id ||
            texture(provinceTex, coord_int + vec2(1.0f / 28160.0f, -1.0f / 10240.0f)) != id ||
            texture(provinceTex, coord_int + vec2(-1.0f / 28160.0f, 1.0f / 10240.0f)) != id ||
            texture(provinceTex, coord_int + vec2(1.0f / 28160.0f, 1.0f / 10240.0f)) != id) {
        color *= vec4(1 - pow(1 - camPos.z, 12), 1 - pow(1 - camPos.z, 12), 1 - pow(1 - camPos.z, 12), 1.0f);
    }
}
