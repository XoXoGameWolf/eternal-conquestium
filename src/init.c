#pragma once

void gameInit() {
    srand(time(NULL));

    window_init("Eternal Conquestium", 1600, 900);
    renderer_init(0.0f, 0.0f, 0.0f);

    window_fullscreen = true;

    map = malloc(sizeof(Map));
    scenario = malloc(sizeof(Scenario));

    mainMenuTex = renderer_createTexture("resources/ui/main_menu.bmp");
    map->provinceTex = renderer_createTexture("resources/map/provinces.bmp");
    map->terrainTex = renderer_createTexture("resources/map/terrain.bmp");
    scenario->borderTex = renderer_createTexture("resources/scenario/borders.bmp");
    scenario->colorTex = renderer_createTexture("resources/scenario/colors.bmp");
    quad = renderer_createMeshFast(
        QUAD_VERTICES, sizeof(QUAD_VERTICES),
        QUAD_COORDS, sizeof(QUAD_COORDS),
        QUAD_INDICES, sizeof(QUAD_INDICES)
    );
    mapShader = renderer_createShader("resources/shaders/map/vert.glsl", "resources/shaders/map/frag.glsl");
    uiShader = renderer_createShader("resources/shaders/ui/vert.glsl", "resources/shaders/ui/frag.glsl");

    camPosX = 0.0f;
    camPosY = 0.0f;
    camPosZ = 1.0f;
    camVelX = 0.0f;
    camVelY = 0.0f;
    camVelZ = 0.0f;

    lastLeftMouseButton = false;

    lastF8 = false;
    lastN = false;
    lastR = false;
    lastEscape = false;
}