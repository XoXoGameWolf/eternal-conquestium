#pragma once

void renderText(float x, float y, float scale, char* text, float r, float g, float b) {
    int length = strlen(text);
    renderer_setUniformTexture(fontShader, "tex", fontTex, 5);
    renderer_setUniformFloat(fontShader, "aspect", aspect);
    int data[length];
    for(int i = 0; i < length; i++) data[i] = text[i];
    renderer_setUniformIntArray(fontShader, "data", data, 32);
    renderer_setUniformInt(fontShader, "length", length);
    renderer_setUniformVec2(fontShader, "pos", x, y);
    renderer_setUniformFloat(fontShader, "scale", scale);
    renderer_setUniformVec3(fontShader, "color", r, g, b);
    renderer_renderMesh(quad, fontShader);
}

void render() {
    renderer_update();

    renderer_setUniformTexture(mapShader, "provinceTex", map->provinceTex, 0);
    renderer_setUniformTexture(mapShader, "terrainTex", map->terrainTex, 1);
    renderer_setUniformTexture(mapShader, "borderTex", scenario->borderTex, 2);
    renderer_setUniformTexture(mapShader, "colorTex", scenario->colorTex, 3);
    renderer_setUniformTexture(mapShader, "centerTex", map->centerTex, 5);
    renderer_setUniformFloat(mapShader, "aspect", aspect);
    renderer_setUniformVec3(mapShader, "camPos", camPosX, camPosY, camPosZ);
    renderer_setUniformFloat(mapShader, "time", glfwGetTime());
    renderer_setUniformFloat(mapShader, "selectedNation", (float)selectedNation / 255.0f);
    renderer_renderMesh(quad, mapShader);

    renderer_setUniformVec3(mapShader, "camPos", camPosX + 2.75f, camPosY, camPosZ);
    renderer_renderMesh(quad, mapShader);

    renderer_setUniformVec3(mapShader, "camPos", camPosX - 2.75f, camPosY, camPosZ);
    renderer_renderMesh(quad, mapShader);

    for(int i = 0; i < 4096; i++) {
        if(armies[i].size == 0) continue;

        float x = (float)armies[i].x / (float)map->provinceTex->width - 0.5f;
        float y = 1 - (float)armies[i].y / (float)map->provinceTex->height - 0.5f;

        x = (2.75 * x - camPosX) / camPosZ;
        y = (y - camPosY) / camPosZ * aspect;

        char buf[8];
        sprintf(buf, "%d", armies[i].size);

        if(selectedArmy == i) {
            renderText(x, y, 0.002f / camPosZ, buf, 1.0, 1.0, 1.0);

        } else {
            renderText(x, y, 0.002f / camPosZ, buf, 0.75, 0.75, 0.75);
        }
    }

    double x;
    double y;
    glfwGetCursorPos(window_window, &x, &y);

    x /= window_width;
    y /= window_height;

    if(mode == 0) {
        renderer_setUniformTexture(uiShader, "tex", mainMenuTex, 4);
        renderer_renderMesh(quad, uiShader);

        renderText(805.0 / 1600.0 * 2 - 1, 2 - 333.0 / 900.0 * 2 - 1, 353.0 / 1600.0 * 0.8, "Play Game", 0, 0, 0);
        renderText(805.0 / 1600.0 * 2 - 1, 2 - 442.0 / 900.0 * 2 - 1, 353.0 / 1600.0 * 0.8, "Terrain Editor", 0, 0, 0);
        renderText(805.0 / 1600.0 * 2 - 1, 2 - 543.0 / 900.0 * 2 - 1, 353.0 / 1600.0 * 0.8, "Border Editor", 0, 0, 0);
        renderText(805.0 / 1600.0 * 2 - 1, 2 - 639.0 / 900.0 * 2 - 1, 353.0 / 1600.0 * 0.8, "Exit Game", 0, 0, 0);

    } else if(mode == 1 || mode == 2 || mode == 5) {
        renderer_setUniformTexture(uiShader, "tex", editorTex, 4);
        renderer_renderMesh(quad, uiShader);

        renderText(85.0 / 1600.0 * 2 - 1, 2 - 40.0 / 900.0 * 2 - 1, 150.0 / 1600.0, "Back", 0, 0, 0);

    } else if(mode == 3) {
        renderer_setUniformTexture(uiShader, "tex", nationSelectTex, 4);
        renderer_renderMesh(quad, uiShader);

        renderText(85.0 / 1600.0 * 2 - 1, 2 - 40.0 / 900.0 * 2 - 1, 150.0 / 1600.0, "Back", 0, 0, 0);
        renderText(817 / 1600.0 * 2 - 1, 2 - 34.0 / 900.0 * 2 - 1, 443.0 / 1600.0 * 0.8, "Select Nation", 0, 0, 0);

    } else if(mode == 4 && selectedNation == 0) {
        renderer_setUniformTexture(uiShader, "tex", gameplayTex, 4);
        renderer_renderMesh(quad, uiShader);

        renderText(110.0 / 1600.0 * 2 - 1, 2 - 43.0 / 900.0 * 2 - 1, 227 / 1600.0 * 0.6, "Menu", 0, 0, 0);
        
    } else if(mode == 4) {
        renderer_setUniformTexture(uiShader, "tex", diplomacyTex, 4);
        renderer_renderMesh(quad, uiShader);

        renderText(110.0 / 1600.0 * 2 - 1, 2 - 43.0 / 900.0 * 2 - 1, 227 / 1600.0 * 0.6, "Menu", 0, 0, 0);
        renderText(151.0 / 1600.0 * 2 - 1, 2 - 275.0 / 900.0 * 2 - 1, 248 / 1600.0 * 0.8, "Diplomacy", 0, 0, 0);
        if(nations[playerNation].wars[selectedNation] || nations[selectedNation].wars[playerNation]) {
            renderText(151.0 / 1600.0 * 2 - 1, 2 - 326.0 / 900.0 * 2 - 1, 253 / 1600.0 * 0.8, "Sue for Peace", 0, 0, 0);

        } else {
            renderText(151.0 / 1600.0 * 2 - 1, 2 - 326.0 / 900.0 * 2 - 1, 253 / 1600.0 * 0.8, "Declare War", 0, 0, 0);
        }
    }
}