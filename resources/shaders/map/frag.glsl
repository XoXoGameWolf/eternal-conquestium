#version 460 core

uniform sampler2D provinceTex;
uniform sampler2D terrainTex;
uniform sampler2D borderTex;
uniform sampler2D colorTex;
uniform vec3 camPos;

in vec2 coord_int;
out vec4 out_color;

const float TERRAIN_NONE = 0.0f / 255.0f;
const float TERRAIN_PLAINS = 1.0f / 255.0f;
const float TERRAIN_OCEAN = 2.0f / 255.0f;

const float NATION_NONE = 0.0f / 255.0f;

vec2 colorToCoord(vec4 color) {
    return vec2((color.b * 255 + 256 * mod(color.g * 255, 16)) / 4096, (color.g * 255 / 16 + 16 * 255 * color.r) / 4096);
}

vec2 getProvinceID(vec2 coord) {
    return colorToCoord(texture(provinceTex, coord));
}

float getTerrainID(vec2 coord) {
    return texture(terrainTex, getProvinceID(coord)).r;
}

float getNationID(vec2 coord) {
    return texture(borderTex, getProvinceID(coord)).r;
}

bool onProvinceBorder(float width) {
    vec2 provinceID = getProvinceID(coord_int);

    return getProvinceID(coord_int + vec2(1.0f  / 28160.0f  * width, 0.0f                    )) != provinceID ||
           getProvinceID(coord_int + vec2(0.0f,                      1.0f  / 10240.0f * width)) != provinceID ||
           getProvinceID(coord_int + vec2(-1.0f / 28160.0f  * width, 0.0f                    )) != provinceID ||
           getProvinceID(coord_int + vec2(0.0f,                      -1.0f / 10240.0f * width)) != provinceID ||
           getProvinceID(coord_int + vec2(-1.0f / 28160.0f  * width, -1.0f / 10240.0f * width)) != provinceID ||
           getProvinceID(coord_int + vec2(1.0f  / 28160.0f  * width, -1.0f / 10240.0f * width)) != provinceID ||
           getProvinceID(coord_int + vec2(-1.0f / 28160.0f  * width, 1.0f  / 10240.0f * width)) != provinceID ||
           getProvinceID(coord_int + vec2(1.0f  / 28160.0f  * width, 1.0f  / 10240.0f * width)) != provinceID;
}

bool onNationBorder(float width) {
    float nationID = getNationID(coord_int);

    return getNationID(coord_int + vec2(1.0f  / 28160.0f * width, 0.0f                    )) != nationID ||
           getNationID(coord_int + vec2(0.0f,                     1.0f  / 10240.0f * width)) != nationID ||
           getNationID(coord_int + vec2(-1.0f / 28160.0f * width, 0.0f                    )) != nationID ||
           getNationID(coord_int + vec2(0.0f,                     -1.0f / 10240.0f * width)) != nationID ||
           getNationID(coord_int + vec2(-1.0f / 28160.0f * width, -1.0f / 10240.0f * width)) != nationID ||
           getNationID(coord_int + vec2(1.0f  / 28160.0f * width, -1.0f / 10240.0f * width)) != nationID ||
           getNationID(coord_int + vec2(-1.0f / 28160.0f * width, 1.0f  / 10240.0f * width)) != nationID ||
           getNationID(coord_int + vec2(1.0f  / 28160.0f * width, 1.0f  / 10240.0f * width)) != nationID;
}

bool onCoast(float width) {
    float terrainID = getTerrainID(coord_int);

    return getTerrainID(coord_int + vec2(1.0f  / 28160.0f * width, 0.0f                    )) != terrainID ||
           getTerrainID(coord_int + vec2(0.0f,                     1.0f  / 10240.0f * width)) != terrainID ||
           getTerrainID(coord_int + vec2(-1.0f / 28160.0f * width, 0.0f                    )) != terrainID ||
           getTerrainID(coord_int + vec2(0.0f,                     -1.0f / 10240.0f * width)) != terrainID ||
           getTerrainID(coord_int + vec2(-1.0f / 28160.0f * width, -1.0f / 10240.0f * width)) != terrainID ||
           getTerrainID(coord_int + vec2(1.0f  / 28160.0f * width, -1.0f / 10240.0f * width)) != terrainID ||
           getTerrainID(coord_int + vec2(-1.0f / 28160.0f * width, 1.0f  / 10240.0f * width)) != terrainID ||
           getTerrainID(coord_int + vec2(1.0f  / 28160.0f * width, 1.0f  / 10240.0f * width)) != terrainID;
}

void main() {
    float terrainID = getTerrainID(coord_int);
    float nationID = getNationID(coord_int);

    if(nationID != NATION_NONE && terrainID != TERRAIN_OCEAN) {
        out_color = texture(colorTex, vec2(nationID, 0));

    } else if(terrainID == TERRAIN_NONE) {
        out_color = vec4(1.0f, 1.0f, 1.0f, 1.0f);

    } else if(terrainID == TERRAIN_PLAINS) {
        out_color = vec4(0.13f, 0.45f, 0.05f, 1.0f);

    } else if(terrainID == TERRAIN_OCEAN) {
        out_color = vec4(0.05f, 0.17f, 0.67f, 1.0f);
    }

    if(onNationBorder(2.5f) || onCoast(2.5f)) {
        out_color *= vec4(0.5f, 0.5f, 0.5f, 1.0f);

    } else if(onProvinceBorder(1.0f)) {
        out_color *= vec4(1 - pow(1 - camPos.z, 8) * 0.5f, 1 - pow(1 - camPos.z, 8) * 0.5f, 1 - pow(1 - camPos.z, 8) * 0.5f, 1.0f);
    }
}