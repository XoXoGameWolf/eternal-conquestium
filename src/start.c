#pragma once

void start() {
    srand(time(NULL));

    fullscreen = true;

    map = malloc(sizeof(Map));
    scenario = malloc(sizeof(Scenario));

    mainMenuTex = createTexture("resources/ui/main_menu.bmp", false);
    editorTex = createTexture("resources/ui/editor.bmp", false);
    nationSelectTex = createTexture("resources/ui/nation_select.bmp", false);
    gameplayTex = createTexture("resources/ui/gameplay.bmp", false);
    diplomacyTex = createTexture("resources/ui/diplomacy.bmp", false);
    editorSelectTex = createTexture("resources/ui/editor_select.bmp", false);

    fontTex = createTexture("resources/font/font.bmp", false);

    map->provinceTex = createTexture("resources/map/provinces.bmp", true);
    map->terrainTex = createTexture("resources/map/terrain.bmp", true);
    map->centerTex = createTexture("resources/map/centers.bmp", true);
    scenario->borderTex = createTexture("resources/scenario/borders.bmp", true);
    scenario->colorTex = createTexture("resources/scenario/colors.bmp", true);
    mapShader = createShader("resources/shaders/map/map_vert.glsl", "resources/shaders/map/map_frag.glsl");
    uiShader = createShader("resources/shaders/ui/ui_vert.glsl", "resources/shaders/ui/ui_frag.glsl");
    fontShader = createShader("resources/shaders/font/font_vert.glsl", "resources/shaders/font/font_frag.glsl");

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

    cam_vel_x = 0.0f;
    cam_vel_y = 0.0f;
    cam_vel_z = 0.0f;

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