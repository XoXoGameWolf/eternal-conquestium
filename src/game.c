#pragma once

typedef struct {
    Texture* provinceTex;
    Texture* terrainTex;
} Map;

typedef struct {
    Texture* borderTex;
    Texture* colorTex;
} Scenario;

Map* map;
Scenario* scenario;

Texture* mainMenuTex;

Shader* mapShader;
Shader* uiShader;

Mesh* quad;

int mode;
float aspect;
int selected;

float camPosX;
float camPosY;
float camPosZ;
float camVelX;
float camVelY;
float camVelZ;

bool lastLeftMouseButton;

bool lastF8;
bool lastN;
bool lastR;
bool lastEscape;