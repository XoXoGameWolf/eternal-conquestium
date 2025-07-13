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
    char nation1;
    char nation2;
} War;

Map* map;
Scenario* scenario;

Texture* mainMenuTex;
Texture* editorTex;
Texture* nationSelectTex;
Texture* gameplayTex;
Texture* diplomacyTex;

Shader* mapShader;
Shader* uiShader;

Mesh* quad;

War* wars;

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

bool lastF8;
bool lastN;
bool lastR;
bool lastEscape;