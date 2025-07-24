#pragma once

void start() {
    srand(time(NULL));
    renderer_init(0.0f, 0.0f, 0.0f);

    window_fullscreen = true;

    map = malloc(sizeof(Map));
    scenario = malloc(sizeof(Scenario));

    mainMenuTex = renderer_createTexture("resources/ui/main_menu.png", false);
    editorTex = renderer_createTexture("resources/ui/editor.png", false);
    nationSelectTex = renderer_createTexture("resources/ui/nation_select.png", false);
    gameplayTex = renderer_createTexture("resources/ui/gameplay.png", false);
    diplomacyTex = renderer_createTexture("resources/ui/diplomacy.png", false);
    editorSelectTex = renderer_createTexture("resources/ui/editor_select.png", false);

    fontTex = renderer_createTexture("resources/font/font.png", false);

    map->provinceTex = renderer_createTexture("resources/map/provinces.png", true);
    map->terrainTex = renderer_createTexture("resources/map/terrain.png", true);
    map->centerTex = renderer_createTexture("resources/map/centers.png", true);
    scenario->borderTex = renderer_createTexture("resources/scenario/borders.png", true);
    scenario->colorTex = renderer_createTexture("resources/scenario/colors.png", true);
    quad = renderer_createMeshFast(
        QUAD_VERTICES, sizeof(QUAD_VERTICES),
        QUAD_COORDS, sizeof(QUAD_COORDS),
        QUAD_INDICES, sizeof(QUAD_INDICES)
    );
    mapShader = renderer_createShader("resources/shaders/map/map_vert.glsl", "resources/shaders/map/map_frag.glsl");
    uiShader = renderer_createShader("resources/shaders/ui/ui_vert.glsl", "resources/shaders/ui/ui_frag.glsl");
    fontShader = renderer_createShader("resources/shaders/font/font_vert.glsl", "resources/shaders/font/font_frag.glsl");

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