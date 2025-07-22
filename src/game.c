#pragma once

typedef struct {
    Texture* provinceTex;
    Texture* centerTex;
    Texture* terrainTex;
} Map;

typedef struct {
    Texture* borderTex;
    Texture* colorTex;
    Texture* borderTexOriginal;
    Texture* colorTexOriginal;
} Scenario;

typedef struct {
    int provinceCount;
    bool wars[256];
} Nation;

typedef struct {
    int x;
    int y;
    int size;
} Army;

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
Army armies[4096];

int mode;
float aspect;
unsigned char selected;
unsigned char selectedNation;
unsigned char playerNation;
int selectedArmy;

float camPosX;
float camPosY;
float camPosZ;
float camVelX;
float camVelY;
float camVelZ;

bool lastLeftMouseButton;
bool lastRightMouseButton;

bool lastN;
bool lastR;