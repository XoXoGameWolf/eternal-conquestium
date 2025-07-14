#pragma once

typedef struct {
    Texture* provinceTex;
    Texture* terrainTex;
} Map;

typedef struct {
    Texture* borderTex;
    Texture* colorTex;
} Scenario;

typedef struct {
    int provinceCount;
    bool wars[256];
} Nation;

Map* map;
Scenario* scenario;

Texture* mainMenuTex;
Texture* editorTex;
Texture* nationSelectTex;
Texture* gameplayTex;
Texture* diplomacyTex;

Texture* fontTex;

Shader* mapShader;
Shader* uiShader;
Shader* fontShader;

Mesh* quad;

Nation nations[256];

int mode;
float aspect;
unsigned char selected;
unsigned char selectedNation;
unsigned char playerNation;

float camPosX;
float camPosY;
float camPosZ;
float camVelX;
float camVelY;
float camVelZ;

bool lastLeftMouseButton;

bool lastN;