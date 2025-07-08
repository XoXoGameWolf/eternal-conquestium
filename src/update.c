#pragma once

void gameUpdate() {
    aspect = (float)window_width / (float)window_height;

    if(glfwGetKey(window_window, GLFW_KEY_W) && mode != 0) { // move camera up
        camVelY += 0.001f * camPosZ;
    }

    if(glfwGetKey(window_window, GLFW_KEY_A) && mode != 0) { // move camera left
        camVelX -= 0.001f * camPosZ;
    }

    if(glfwGetKey(window_window, GLFW_KEY_S) && mode != 0) { // move camera down
        camVelY -= 0.001f * camPosZ;
    }

    if(glfwGetKey(window_window, GLFW_KEY_D) && mode != 0) { // move camera right
        camVelX += 0.001f * camPosZ;
    }

    if(glfwGetKey(window_window, GLFW_KEY_E) && mode != 0) { // zoom camera in
        camVelZ -= 0.002 * camPosZ;
    }

    if(glfwGetKey(window_window, GLFW_KEY_Q) && mode != 0) { // zoom camera out
        camVelZ += 0.002 * camPosZ;
    }

    // velocity
    camPosX += camVelX;
    camPosY += camVelY;
    camPosZ += camVelZ;

    // friction
    camVelX *= 0.9f;
    camVelY *= 0.9f;
    camVelZ *= 0.9f;


    // maximum zoom (whole map)
    if(camPosZ > 0.5 * aspect) {
        camPosZ = 0.5 * aspect;
    }

    // lock y below top of map
    if(camPosY > 0.5 * ((0.5 * aspect) - camPosZ) / (0.5 * aspect)) {
        camPosY = 0.5 * ((0.5 * aspect) - camPosZ) / (0.5 * aspect);
    }

    // lock y above bottom of map
    if(camPosY < -0.5 * ((0.5 * aspect) - camPosZ) / (0.5 * aspect)) {
        camPosY = -0.5 * ((0.5 * aspect) - camPosZ) / (0.5 * aspect);
    }

    // loop x when rightmost
    if(camPosX > 2.75f) {
        camPosX = -2.75f + (camPosX - 2.75f);
    }

    // loop x when leftmost
    if(camPosX < -2.75f) {
        camPosX = 2.75f + (camPosX + 2.75f);
    }

    if(glfwGetMouseButton(window_window, GLFW_MOUSE_BUTTON_LEFT)) { // set province
        double x;
        double y;

        glfwGetCursorPos(window_window, &x, &y);

        if(mode == 0 && !lastLeftMouseButton) { // main menu
            int tx = (int)floor(x / window_width * mainMenuTex->width);
            int ty = (int)floor(y / window_height * mainMenuTex->height);

            if(tx > 629 && tx < 982 && ty > 286 && ty < 378) {
                mode = 1;
            }
            if(tx > 629 && tx < 982 && ty > 398 && ty < 486) {
                mode = 2;
            }
            if(tx > 629 && tx < 982 && ty > 507 && ty < 586) {
                window_open = false;
            }

            lastLeftMouseButton = glfwGetMouseButton(window_window, GLFW_MOUSE_BUTTON_LEFT);

        } else if((mode == 1 || mode == 2) && !lastLeftMouseButton) { // terrain or border editor
            x = x / window_width * 2 - 1;
            y = 2 - y / window_height * 2 - 1;

            x = (x * camPosZ + camPosX) / 2.75;
            if(x > 0.5) x -= 1;
            if(x < -0.5) x += 1;
            y = y / aspect * camPosZ + camPosY;

            if(x > -0.5f && x < 0.5f && y > -0.5f && y < 0.5f) {
                int tx = (int)floor((x + 0.5) * map->provinceTex->width);
                int ty = (int)floor((-y + 0.5) * map->provinceTex->height);

                int address = (tx + ty * map->provinceTex->width) * map->provinceTex->channels;

                unsigned char r = map->provinceTex->data[address];
                unsigned char g = map->provinceTex->data[address + 1];
                unsigned char b = map->provinceTex->data[address + 2];

                tx = b + 256 * (g % 16);
                ty = g / 16 + 16 * r;
                   
                if(mode == 1) { // terrain editor
                    address = (tx + ty * map->terrainTex->width) * map->terrainTex->channels;
                        
                    if(map->terrainTex->data[address] != selected) {
                        map->terrainTex->data[address] = selected;
                        map->terrainTex->data[address + 1] = 0;
                        map->terrainTex->data[address + 2] = 0;
                        renderer_updateTexture(map->terrainTex);

                        if(selected == 2) {
                            scenario->borderTex->data[address] = 0;
                            scenario->borderTex->data[address + 1] = 0;
                            scenario->borderTex->data[address + 2] = 0;
                            renderer_updateTexture(scenario->borderTex);
                        }
                    }
                    // border editor
                } else if(mode == 2 && map->terrainTex->data[(tx + ty * map->terrainTex->width) * map->terrainTex->channels] != 2) {
                    address = (tx + ty * scenario->borderTex->width) * scenario->borderTex->channels;
                        
                    if(scenario->borderTex->data[address] != selected) {
                        scenario->borderTex->data[address] = selected;
                        scenario->borderTex->data[address + 1] = 0;
                        scenario->borderTex->data[address + 2] = 0;
                        renderer_updateTexture(scenario->borderTex);
                    }
                }
            }
        }
    } else {
        lastLeftMouseButton = false;
    }

    if(glfwGetMouseButton(window_window, GLFW_MOUSE_BUTTON_RIGHT)) { // pick province
        double x;
        double y;

        glfwGetCursorPos(window_window, &x, &y);
        x = x / window_width * 2 - 1;
        y = 2 - y / window_height * 2 - 1;

        x = (x * camPosZ + camPosX) / 2.75;
        if(x > 0.5) x -= 1;
        if(x < -0.5) x += 1;
        y = y / aspect * camPosZ + camPosY;

       if(x > -0.5f && x < 0.5f && y > -0.5f && y < 0.5f) {
            int tx = (int)floor((x + 0.5) * map->provinceTex->width);
            int ty = (int)floor((-y + 0.5) * map->provinceTex->height);

            int address = (tx + ty * map->provinceTex->width) * map->provinceTex->channels;

            unsigned char r = map->provinceTex->data[address];
            unsigned char g = map->provinceTex->data[address + 1];
            unsigned char b = map->provinceTex->data[address + 2];

            tx = b + 256 * (g % 16);
            ty = g / 16 + 16 * r;

            if(mode == 1) { // terrain editor
                address = (tx + ty * map->terrainTex->width) * map->terrainTex->channels;
                selected = map->terrainTex->data[address];

            } else if(mode == 2) { // border editor
                address = (tx + ty * scenario->borderTex->width) * scenario->borderTex->channels;
                selected = scenario->borderTex->data[address];
            }
        }
    }

    if(glfwGetKey(window_window, GLFW_KEY_ESCAPE) && !lastEscape) { // go back to main menu
        mode = 0;
    }
    lastEscape = glfwGetKey(window_window, GLFW_KEY_ESCAPE);

    if(glfwGetKey(window_window, GLFW_KEY_R) && !lastR) { // reload data
        renderer_deleteTexture(map->terrainTex);
        renderer_deleteTexture(scenario->borderTex);
        renderer_deleteTexture(scenario->colorTex);
        map->terrainTex = renderer_createTexture("resources/map/terrain.bmp");
        scenario->borderTex = renderer_createTexture("resources/scenario/borders.bmp");
        scenario->colorTex = renderer_createTexture("resources/scenario/colors.bmp");
    }
    lastR = glfwGetKey(window_window, GLFW_KEY_R);

    if(glfwGetKey(window_window, GLFW_KEY_N) && !lastN) { // select new nation
        if(mode == 2) {
            selected = 1;

            while(!scenario->colorTex->data[selected * scenario->colorTex->channels] == 0 ||
                    !scenario->colorTex->data[selected * scenario->colorTex->channels + 1] == 0 ||
                    !scenario->colorTex->data[selected * scenario->colorTex->channels + 2] == 0) {
                selected++;
            }

            scenario->colorTex->data[selected * scenario->colorTex->channels] = rand() % 256;
            scenario->colorTex->data[selected * scenario->colorTex->channels + 1] = rand() % 256;
            scenario->colorTex->data[selected * scenario->colorTex->channels + 2] = rand() % 256;
            renderer_updateTexture(scenario->colorTex);
        }
    }
    lastN = glfwGetKey(window_window, GLFW_KEY_N);

    if(glfwGetKey(window_window, GLFW_KEY_F8) && !lastF8) { // save
        renderer_saveTexture("resources/map/terrain.bmp", map->terrainTex);
        renderer_saveTexture("resources/scenario/borders.bmp", scenario->borderTex);
        renderer_saveTexture("resources/scenario/colors.bmp", scenario->colorTex);
    }
    lastF8 = glfwGetKey(window_window, GLFW_KEY_F8);
}