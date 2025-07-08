#pragma once

void gameRender() {
    renderer_update();

    renderer_setUniformTexture(mapShader, "provinceTex", map->provinceTex, 0);
    renderer_setUniformTexture(mapShader, "terrainTex", map->terrainTex, 1);
    renderer_setUniformTexture(mapShader, "borderTex", scenario->borderTex, 2);
    renderer_setUniformTexture(mapShader, "colorTex", scenario->colorTex, 3);
    renderer_setUniformFloat(mapShader, "aspect", aspect);
    renderer_setUniformVec3(mapShader, "camPos", camPosX, camPosY, camPosZ);
    renderer_renderMesh(quad, mapShader);

    renderer_setUniformVec3(mapShader, "camPos", camPosX + 2.75f, camPosY, camPosZ);
    renderer_renderMesh(quad, mapShader);

    renderer_setUniformVec3(mapShader, "camPos", camPosX - 2.75f, camPosY, camPosZ);
    renderer_renderMesh(quad, mapShader);

    if(mode == 0) {
        renderer_setUniformTexture(uiShader, "tex", mainMenuTex, 4);
        renderer_renderMesh(quad, uiShader);
    }

    window_update();
}