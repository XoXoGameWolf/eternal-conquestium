#pragma once

void gameInit() {
    srand(time(NULL));

    window_init("Eternal Conquestium", 1600, 900);
    renderer_init(0.0f, 0.0f, 0.0f);

    window_fullscreen = true;

    map = malloc(sizeof(Map));
    scenario = malloc(sizeof(Scenario));

    mainMenuTex = renderer_createTexture("resources/ui/main_menu.bmp");
    editorTex = renderer_createTexture("resources/ui/editor.bmp");
    nationSelectTex = renderer_createTexture("resources/ui/nation_select.bmp");
    gameplayTex = renderer_createTexture("resources/ui/gameplay.bmp");
    diplomacyTex = renderer_createTexture("resources/ui/diplomacy.bmp");

    fontTex = renderer_createTexture("resources/font/font.bmp");

    map->provinceTex = renderer_createTexture("resources/map/provinces.bmp");
    map->terrainTex = renderer_createTexture("resources/map/terrain.bmp");
    map->centerTex = renderer_createTexture("resources/map/centers.bmp");
    scenario->borderTex = renderer_createTexture("resources/scenario/borders.bmp");
    scenario->colorTex = renderer_createTexture("resources/scenario/colors.bmp");
    quad = renderer_createMeshFast(
        QUAD_VERTICES, sizeof(QUAD_VERTICES),
        QUAD_COORDS, sizeof(QUAD_COORDS),
        QUAD_INDICES, sizeof(QUAD_INDICES)
    );
    mapShader = renderer_createShader("resources/shaders/map/vert.glsl", "resources/shaders/map/frag.glsl");
    uiShader = renderer_createShader("resources/shaders/ui/vert.glsl", "resources/shaders/ui/frag.glsl");
    fontShader = renderer_createShader("resources/shaders/font/vert.glsl", "resources/shaders/font/frag.glsl");

    for(int i = 0; i < 256; i++) {
        nations[i].provinceCount = 0;

        for(int j = 0; j < 256; j++) {
            nations[i].wars[j] = false;
        }
    }

    for(int x = 0; x < scenario->borderTex->width; x++) {
        for(int y = 0; y < scenario->borderTex->height; y++) {
            int address = (x + scenario->borderTex->width * y) * scenario->borderTex->channels;
            nations[(unsigned char)scenario->borderTex->data[address]].provinceCount++;
        }
    }

    for(int i = 0; i < 4096; i++) {
        armies[i].x = 0;
        armies[i].y = 0;
        armies[i].size = 0;
    }

    camPosX = 0.0f;
    camPosY = 0.0f;
    camPosZ = 1.0f;
    camVelX = 0.0f;
    camVelY = 0.0f;
    camVelZ = 0.0f;

    mode = 0;
    aspect = 0;
    selected = 0;
    selectedNation = 0;
    playerNation = 0;
    selectedArmy = -1;

    lastLeftMouseButton = false;
    lastRightMouseButton = false;

    lastN = false;
    lastR = false;
}