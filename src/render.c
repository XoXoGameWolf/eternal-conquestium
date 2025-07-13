#pragma once

void gameRender() {
    renderer_update();

    renderer_setUniformTexture(mapShader, "provinceTex", map->provinceTex, 0);
    renderer_setUniformTexture(mapShader, "terrainTex", map->terrainTex, 1);
    renderer_setUniformTexture(mapShader, "borderTex", scenario->borderTex, 2);
    renderer_setUniformTexture(mapShader, "colorTex", scenario->colorTex, 3);
    renderer_setUniformFloat(mapShader, "aspect", aspect);
    renderer_setUniformVec3(mapShader, "camPos", camPosX, camPosY, camPosZ);
    renderer_setUniformFloat(mapShader, "time", glfwGetTime());
    renderer_setUniformFloat(mapShader, "selectedNation", (float)selectedNation / 255.0f);
    renderer_renderMesh(quad, mapShader);

    renderer_setUniformVec3(mapShader, "camPos", camPosX + 2.75f, camPosY, camPosZ);
    renderer_renderMesh(quad, mapShader);

    renderer_setUniformVec3(mapShader, "camPos", camPosX - 2.75f, camPosY, camPosZ);
    renderer_renderMesh(quad, mapShader);

    double x;
    double y;
    glfwGetCursorPos(window_window, &x, &y);

    x /= window_width;
    y /= window_height;

    if(mode == 0) {
        if(x > 629.0f / 1600.0f && x < 982.0f / 1600.0f && y > 286.0f / 900.0f && y < 378.0f / 900.0f) {
            renderer_setUniformVec2(uiShader, "highlight1", 629.0f / 1600.0f, 286.0f / 900.0f);
            renderer_setUniformVec2(uiShader, "highlight2", 982.0f / 1600.0f, 378.0f / 900.0f);

        } else if(x > 629.0f / 1600.0f && x < 982.0f / 1600.0f && y > 394.0f / 900.0f && y < 486.0f / 900.0f) {
            renderer_setUniformVec2(uiShader, "highlight1", 629.0f / 1600.0f, 394.0f / 900.0f);
            renderer_setUniformVec2(uiShader, "highlight2", 982.0f / 1600.0f, 486.0f / 900.0f);

        } else if(x > 629.0f / 1600.0f && x < 982.0f / 1600.0f && y > 498.0f / 900.0f && y < 586.0f / 900.0f) {
            renderer_setUniformVec2(uiShader, "highlight1", 629.0f / 1600.0f, 498.0f / 900.0f);
            renderer_setUniformVec2(uiShader, "highlight2", 982.0f / 1600.0f, 586.0f / 900.0f);

        } else if(x > 629.0f / 1600.0f && x < 982.0f / 1600.0f && y > 597.0f / 900.0f && y < 676.0f / 900.0f) {
            renderer_setUniformVec2(uiShader, "highlight1", 629.0f / 1600.0f, 597.0f / 900.0f);
            renderer_setUniformVec2(uiShader, "highlight2", 982.0f / 1600.0f, 676.0f / 900.0f);

        } else {
            renderer_setUniformVec2(uiShader, "highlight1", 0.0f, 0.0f);
            renderer_setUniformVec2(uiShader, "highlight2", 0.0f, 0.0f);
        }

        renderer_setUniformTexture(uiShader, "tex", mainMenuTex, 4);
        renderer_renderMesh(quad, uiShader);

    } else if(mode == 1 || mode == 2) {
        if(x > 0.0f && x < 156.0f / 1600.0f && y > 0.0f && y < 53.0f / 900.0f) {
            renderer_setUniformVec2(uiShader, "highlight1", 0.0f, 0.0f);
            renderer_setUniformVec2(uiShader, "highlight2", 156.0f / 1600.0f, 53.0f / 900.0f);

        } else {
            renderer_setUniformVec2(uiShader, "highlight1", 0.0f, 0.0f);
            renderer_setUniformVec2(uiShader, "highlight2", 0.0f, 0.0f);
        }

        renderer_setUniformTexture(uiShader, "tex", editorTex, 4);
        renderer_renderMesh(quad, uiShader);

    } else if(mode == 3) {
        if(x > 0.0f && x < 156.0f / 1600.0f && y > 0.0f && y < 53.0f / 900.0f) {
            renderer_setUniformVec2(uiShader, "highlight1", 0.0f, 0.0f);
            renderer_setUniformVec2(uiShader, "highlight2", 156.0f / 1600.0f, 53.0f / 900.0f);
        
        } else if(x > 598.0f / 1600.0f && x < 1042.0f / 1600.0f && y > 0.0f && y < 68.0f / 900.0f) {
            renderer_setUniformVec2(uiShader, "highlight1", 598.0f / 1600.0f, 0.0f);
            renderer_setUniformVec2(uiShader, "highlight2", 1024.0f / 1600.0f, 68.0f / 900.0f);

        } else {
            renderer_setUniformVec2(uiShader, "highlight1", 0.0f, 0.0f);
            renderer_setUniformVec2(uiShader, "highlight2", 0.0f, 0.0f);
        }
        renderer_setUniformTexture(uiShader, "tex", nationSelectTex, 4);
        renderer_renderMesh(quad, uiShader);

    } else if(mode == 4 && selectedNation == 0) {
        if(x > 0.0f && x < 227.0f / 1600.0f && y > 0.0f && y < 79.0f / 900.0f) {
            renderer_setUniformVec2(uiShader, "highlight1", 0.0f, 0.0f);
            renderer_setUniformVec2(uiShader, "highlight2", 227.0f / 1600.0f, 79.0f / 900.0f);

        } else {
            renderer_setUniformVec2(uiShader, "highlight1", 0.0f, 0.0f);
            renderer_setUniformVec2(uiShader, "highlight2", 0.0f, 0.0f);
        }
        renderer_setUniformTexture(uiShader, "tex", gameplayTex, 4);
        renderer_renderMesh(quad, uiShader);
        
    } else if(mode == 4) {
        if(x > 0.0f && x < 227.0f / 1600.0f && y > 0.0f && y < 79.0f / 900.0f) {
            renderer_setUniformVec2(uiShader, "highlight1", 0.0f, 0.0f);
            renderer_setUniformVec2(uiShader, "highlight2", 227.0f / 1600.0f, 79.0f / 900.0f);
        
        } else if(x > 273.0f / 1600.0f && x < 311.0f / 1600.0f && y > 241.0f / 900.0f && y < 277.0f / 900.0f) {
            renderer_setUniformVec2(uiShader, "highlight1", 273.0f / 1600.0f, 241.0f / 900.0f);
            renderer_setUniformVec2(uiShader, "highlight2", 311.0f / 1600.0f, 277.0f / 900.0f);
        
        } else if(x > 25.0f / 1600.0f && x < 278.0f / 1600.0f && y > 303.0f / 900.0f && y < 346.0f / 900.0f) {
            renderer_setUniformVec2(uiShader, "highlight1", 25.0f / 1600.0f, 303.0f / 900.0f);
            renderer_setUniformVec2(uiShader, "highlight2", 278.0f / 1600.0f, 346.0f / 900.0f);
        
        } else if(x > 25.0f / 1600.0f && x < 278.0f / 1600.0f && y > 351.0f / 900.0f && y < 394.0f / 900.0f) {
            renderer_setUniformVec2(uiShader, "highlight1", 25.0f / 1600.0f, 351.0f / 900.0f);
            renderer_setUniformVec2(uiShader, "highlight2", 278.0f / 1600.0f, 394.0f / 900.0f);
        
        } else if(x > 25.0f / 1600.0f && x < 278.0f / 1600.0f && y > 400.0f / 900.0f && y < 443.0f / 900.0f) {
            renderer_setUniformVec2(uiShader, "highlight1", 25.0f / 1600.0f, 400.0f / 900.0f);
            renderer_setUniformVec2(uiShader, "highlight2", 278.0f / 1600.0f, 443.0f / 900.0f);
        
        } else if(x > 25.0f / 1600.0f && x < 278.0f / 1600.0f && y > 449.0f / 900.0f && y < 492.0f / 900.0f) {
            renderer_setUniformVec2(uiShader, "highlight1", 25.0f / 1600.0f, 449.0f / 900.0f);
            renderer_setUniformVec2(uiShader, "highlight2", 278.0f / 1600.0f, 492.0f / 900.0f);
        
        } else if(x > 25.0f / 1600.0f && x < 278.0f / 1600.0f && y > 499.0f / 900.0f && y < 542.0f / 900.0f) {
            renderer_setUniformVec2(uiShader, "highlight1", 25.0f / 1600.0f, 499.0f / 900.0f);
            renderer_setUniformVec2(uiShader, "highlight2", 278.0f / 1600.0f, 542.0f / 900.0f);
        
        } else if(x > 25.0f / 1600.0f && x < 278.0f / 1600.0f && y > 550.0f / 900.0f && y < 593.0f / 900.0f) {
            renderer_setUniformVec2(uiShader, "highlight1", 25.0f / 1600.0f, 550.0f / 900.0f);
            renderer_setUniformVec2(uiShader, "highlight2", 278.0f / 1600.0f, 593.0f / 900.0f);
        
        } else if(x > 25.0f / 1600.0f && x < 278.0f / 1600.0f && y > 600.0f / 900.0f && y < 643.0f / 900.0f) {
            renderer_setUniformVec2(uiShader, "highlight1", 25.0f / 1600.0f, 600.0f / 900.0f);
            renderer_setUniformVec2(uiShader, "highlight2", 278.0f / 1600.0f, 643.0f / 900.0f);
        
        } else if(x > 25.0f / 1600.0f && x < 278.0f / 1600.0f && y > 650.0f / 900.0f && y < 693.0f / 900.0f) {
            renderer_setUniformVec2(uiShader, "highlight1", 25.0f / 1600.0f, 650.0f / 900.0f);
            renderer_setUniformVec2(uiShader, "highlight2", 278.0f / 1600.0f, 693.0f / 900.0f);

        } else {
            renderer_setUniformVec2(uiShader, "highlight1", 0.0f, 0.0f);
            renderer_setUniformVec2(uiShader, "highlight2", 0.0f, 0.0f);
        }
        renderer_setUniformTexture(uiShader, "tex", diplomacyTex, 4);
        renderer_renderMesh(quad, uiShader);
    }

    window_update();
}