#pragma once

void update() {
    aspect = (float)window_width / (float)window_height;

    if(glfwGetKey(window_window, GLFW_KEY_W) && mode != 0 && mode != 6) { // move camera up
        camVelY += 0.001f * camPosZ;
    }

    if(glfwGetKey(window_window, GLFW_KEY_A) && mode != 0 && mode != 6) { // move camera left
        camVelX -= 0.001f * camPosZ;
    }

    if(glfwGetKey(window_window, GLFW_KEY_S) && mode != 0 && mode != 6) { // move camera down
        camVelY -= 0.001f * camPosZ;
    }

    if(glfwGetKey(window_window, GLFW_KEY_D) && mode != 0 && mode != 6) { // move camera right
        camVelX += 0.001f * camPosZ;
    }

    if(glfwGetKey(window_window, GLFW_KEY_E) && mode != 0 && mode != 6) { // zoom camera in
        camVelZ -= 0.002 * camPosZ;
    }

    if(glfwGetKey(window_window, GLFW_KEY_Q) && mode != 0 && mode != 6) { // zoom camera out
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

    if(mode == 4 && glfwGetKey(window_window, GLFW_KEY_R) && !lastR) {
        double x;
        double y;

        glfwGetCursorPos(window_window, &x, &y);

        x = x / window_width * 2 - 1;
        y = 2 - y / window_height * 2 - 1;

        x = (x * camPosZ + camPosX) / 2.75f;
        if(x > 1) x -= 2;
        if(x < -1) x += 2;
        y = y / aspect * camPosZ + camPosY;

        x = x + 0.5;
        y = -y + 0.5;

        int tx = (int)(x * map->provinceTex->width);
        int ty = (int)(y * map->provinceTex->height);

        int address = (tx + map->provinceTex->width * ty) * map->provinceTex->channels;

        unsigned char r = map->provinceTex->data[address];
        unsigned char g = map->provinceTex->data[address + 1];
        unsigned char b = map->provinceTex->data[address + 2];

        tx = b + 256 * (g % 16);
        ty = g / 16 + 16 * r;

        address = (tx + scenario->borderTex->width * ty) * scenario->borderTex->channels;

        if(scenario->borderTex->data[address] == (char)playerNation) {
            int tx2 = 0;
            int ty2 = 0;

            for(tx = 0; tx < map->centerTex->width; tx++) {
                for(ty = 0; ty < map->centerTex->height; ty++) {
                    address = (tx + map->centerTex->width * ty) * map->centerTex->channels;

                    if(map->centerTex->data[address] == 0) continue;

                    if(map->provinceTex->data[address] != (char)r || 
                            map->provinceTex->data[address + 1] != (char)g || 
                            map->provinceTex->data[address + 2] != (char)b) {
                        continue;
                    }

                    tx2 = tx;
                    ty2 = ty;
                    break;
                }

                if(tx2 != 0 || ty2 != 0) break;
            }

            if(tx2 != 0 || ty2 != 0) {
                for(int i = 0; i < 4096; i++) {
                    if(armies[i].x == tx2 && armies[i].y == ty2) {
                        armies[i].size += rand() % 500 + 750;
                        break;
                    }

                    if(armies[i].size == 0) {
                        armies[i].x = tx2;
                        armies[i].y = ty2;
                        armies[i].size = rand() % 500 + 750;
                        break;
                    }
                }
            }
        }
    }
    lastR = glfwGetKey(window_window, GLFW_KEY_R);

    if(glfwGetMouseButton(window_window, GLFW_MOUSE_BUTTON_LEFT)) { // set province
        double x;
        double y;

        glfwGetCursorPos(window_window, &x, &y);

        if(mode == 5 && !lastLeftMouseButton) { // center editor
            int tx = (int)floor(x / window_width * editorTex->width);
            int ty = (int)floor(y / window_height * editorTex->height);

            if(tx > 0 && tx < 291 && ty > 0 && ty < 90) {
                renderer_saveTexture("resources/map/terrain.png", map->terrainTex);
                renderer_saveTexture("resources/map/centers.png", map->centerTex);
                renderer_saveTexture("resources/scenario/borders.png", scenario->borderTex);
                renderer_saveTexture("resources/scenario/colors.png", scenario->colorTex);

                mode = 6;
                selected = 0;
                selectedNation = 0;
                playerNation = 0;

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

            } else {
                x = x / (float)window_width * 2 - 1;
                y = 2 - y / (float)window_height * 2 - 1;
            
                x = (x * camPosZ + camPosX) / 2.75f;
                if(x < -1) x += 2;
                if(x > 1) x -= 2;
                y = y / aspect * camPosZ + camPosY;

                tx = (int)floor((x + 0.5) * (float)map->centerTex->width);
                ty = (int)floor((-y + 0.5) * (float)map->centerTex->height);

                int address = (tx + map->centerTex->width * ty) * map->centerTex->channels;

                map->centerTex->data[address] = (char)255;
                map->centerTex->data[address + 1] = (char)255;
                map->centerTex->data[address + 2] = (char)255;
                renderer_updateTexture(map->centerTex, true);
            }

            lastLeftMouseButton = true;

        } else if(mode == 0 && !lastLeftMouseButton) { // main menu
            int tx = (int)floor(x / window_width * mainMenuTex->width);
            int ty = (int)floor(y / window_height * mainMenuTex->height);

            if(tx > 805 && tx < 1257 && ty > 325 && ty < 430) {
                mode = 3;
            }
            if(tx > 805 && tx < 1257 && ty > 448 && ty < 553) {
                mode = 6;
            }
            if(tx > 805 && tx < 1257 && ty > 567 && ty < 667) {
                window_open = false;
            }

            lastLeftMouseButton = true;
        
        } else if(mode == 4 && !lastLeftMouseButton) { // gameplay
            int tx = (int)floor(x / window_width * gameplayTex->width);
            int ty = (int)floor(y / window_height * gameplayTex->height);

            if(tx > 0 && tx < 291 && ty > 0 && ty < 90) {
                mode = 0;
                selectedNation = 0;
                playerNation = 0;

                for(int i = 0; i < 256; i++) {
                    nations[i].provinceCount = 0;

                    for(int j = 0; j < 256; j++) {
                        nations[i].wars[j] = false;
                    }
                }

                scenario->borderTex = renderer_createTexture("resources/scenario/borders.png", true);
                scenario->colorTex = renderer_createTexture("resources/scenario/colors.png", true);

                for(int x = 0; x < scenario->borderTex->width; x++) {
                    for(int y = 0; y < scenario->borderTex->height; y++) {
                        int address = (x + scenario->borderTex->width * y) * scenario->borderTex->channels;
                        nations[(unsigned char)scenario->borderTex->data[address]].provinceCount++;
                    }
                }

                for(int i = 0; i < 4096; i++) {
                    armies[i].x = 0;
                    armies[i].y = 0;
                }

            } else if(selectedNation != 0 && tx > 352 && tx < 398 && ty > 274 && ty < 317) {
                selectedNation = 0;
            
            } else if(selectedNation != 0 && tx > 32 && tx < 356 && ty > 345 && ty < 394) {
                bool war = nations[playerNation].wars[selectedNation] || nations[selectedNation].wars[playerNation];

                if(war) {
                    nations[playerNation].wars[selectedNation] = false;
                    nations[selectedNation].wars[playerNation] = false;

                } else {
                    nations[playerNation].wars[selectedNation] = true;
                    nations[selectedNation].wars[playerNation] = true;
                }

            } else if(selectedNation != 0 && tx > 0 && tx < 398 && ty > 274 && ty < 802) {


            } else {
                x = x / window_width * 2 - 1;
                y = 2 - y / window_height * 2 - 1;

                x = (x * camPosZ + camPosX) / 2.75;
                if(x > 0.5) x -= 1;
                if(x < -0.5) x += 1;
                y = y / aspect * camPosZ + camPosY;

                tx = (int)floor((x + 0.5) * map->provinceTex->width);
                ty = (int)floor((-y + 0.5) * map->provinceTex->height);

                int address = (tx + map->provinceTex->width * ty) * map->provinceTex->channels;

                unsigned char r = map->provinceTex->data[address];
                unsigned char g = map->provinceTex->data[address + 1];
                unsigned char b = map->provinceTex->data[address + 2];

                tx = b + 256 * (g % 16);
                ty = g / 16 + 16 * r;

                address = (tx + scenario->borderTex->width * ty) * scenario->borderTex->channels;

                selectedNation = scenario->borderTex->data[address];
                if(selectedNation == playerNation) selectedNation = 0;
            }

            lastLeftMouseButton = true;

        } else if(mode == 3 && !lastLeftMouseButton) { // nation selection
            int tx = (int)floor(x / window_width * nationSelectTex->width);
            int ty = (int)floor(y / window_height * nationSelectTex->height);

            if(tx > 0 && tx < 291 && ty > 0 && ty < 90) {
                mode = 0;
                selectedNation = 0;

            } else if(tx > 765 && tx < 1334 && ty > 0 && ty < 77) {
                if(selectedNation != 0) {
                    playerNation = selectedNation;
                    selectedNation = 0;
                    mode = 4;
                }

            } else {
                x = x / window_width * 2 - 1;
                y = 2 - y / window_height * 2 - 1;

                x = (x * camPosZ + camPosX) / 2.75;
                if(x > 0.5) x -= 1;
                if(x < -0.5) x += 1;
                y = y / aspect * camPosZ + camPosY;
                
                tx = (int)floor((x + 0.5) * map->provinceTex->width);
                ty = (int)floor((-y + 0.5) * map->provinceTex->height);

                int address = (tx + ty * map->provinceTex->width) * map->provinceTex->channels;

                unsigned char r = map->provinceTex->data[address];
                unsigned char g = map->provinceTex->data[address + 1];
                unsigned char b = map->provinceTex->data[address + 2];

                tx = b + 256 * (g % 16);
                ty = g / 16 + 16 * r;

                address = (tx + ty * scenario->borderTex->width) * scenario->borderTex->channels;

                if(scenario->borderTex->data[address] != 0) {
                    selectedNation = scenario->borderTex->data[address];
                }
            }

            lastLeftMouseButton = true;

        } else if((mode == 1 || mode == 2) && !lastLeftMouseButton) { // terrain or border editor
            int tx = (int)floor(x / window_width * editorTex->width);
            int ty = (int)floor(y / window_height * editorTex->height);

            if(tx > 0 && tx < 291 && ty > 0 && ty < 90) {
                renderer_saveTexture("resources/map/terrain.png", map->terrainTex);
                renderer_saveTexture("resources/map/centers.png", map->centerTex);
                renderer_saveTexture("resources/scenario/borders.png", scenario->borderTex);
                renderer_saveTexture("resources/scenario/colors.png", scenario->colorTex);

                mode = 6;
                selected = 0;
                selectedNation = 0;
                playerNation = 0;

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

            } else {
                x = x / window_width * 2 - 1;
                y = 2 - y / window_height * 2 - 1;

                x = (x * camPosZ + camPosX) / 2.75;
                if(x > 0.5) x -= 1;
                if(x < -0.5) x += 1;
                y = y / aspect * camPosZ + camPosY;

                if(x > -0.5f && x < 0.5f && y > -0.5f && y < 0.5f) {
                    tx = (int)floor((x + 0.5) * map->provinceTex->width);
                    ty = (int)floor((-y + 0.5) * map->provinceTex->height);

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
                            renderer_updateTexture(map->terrainTex, true);

                            if(selected == 2) {
                                scenario->borderTex->data[address] = 0;
                                scenario->borderTex->data[address + 1] = 0;
                                scenario->borderTex->data[address + 2] = 0;
                                renderer_updateTexture(scenario->borderTex, true);
                            }
                        }
                        // border editor
                    } else if(mode == 2 && map->terrainTex->data[(tx + ty * map->terrainTex->width) * map->terrainTex->channels] != 2) {
                        address = (tx + ty * scenario->borderTex->width) * scenario->borderTex->channels;
                            
                        if(scenario->borderTex->data[address] != (char)selected) {
                            scenario->borderTex->data[address] = selected;
                            scenario->borderTex->data[address + 1] = 0;
                            scenario->borderTex->data[address + 2] = 0;
                            renderer_updateTexture(scenario->borderTex, true);
                        }
                    }
                }
            }

        } else if(mode == 6 && !lastLeftMouseButton) {
            int tx = (int)floor(x / window_width * editorSelectTex->width);
            int ty = (int)floor(y / window_height * editorSelectTex->height);

            if(tx > 772 && tx < 1304 && ty > 245 && ty < 361) {
                mode = 1;

            } else if(tx > 772 && tx < 1304 && ty > 395 && ty < 511) {
                mode = 2;

            } else if(tx > 772 && tx < 1304 && ty > 545 && ty < 661) {
                mode = 5;

            } else if(tx > 772 && tx < 1304 && ty > 695 && ty < 811) {
                mode = 0;
            }

            lastLeftMouseButton = true;
        }

    } else {
        lastLeftMouseButton = false;
    }

    if(glfwGetMouseButton(window_window, GLFW_MOUSE_BUTTON_RIGHT)) { // right click
        if(!lastRightMouseButton) {
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

                } else if(mode == 4) { // select army
                    int address2 = (tx + scenario->borderTex->width * ty) * scenario->borderTex->channels;

                    bool valid = false;

                    if(scenario->borderTex->data[address2] == (char)playerNation) valid = true;
                    if(nations[playerNation].wars[(unsigned char)scenario->borderTex->data[address2]]) valid = true;
                    if(nations[(unsigned char)scenario->borderTex->data[address2]].wars[playerNation]) valid = true;

                    if(valid) {
                        tx = 0;
                        ty = 0;

                        for(int x2 = 0; x2 < map->centerTex->width; x2++) {
                            for(int y2 = 0; y2 < map->centerTex->height; y2++) {
                                int address = (x2 + map->provinceTex->width * y2) * map->provinceTex->channels;
                                if(map->provinceTex->data[address] != (char)r 
                                        || map->provinceTex->data[address + 1] != (char)g
                                        || map->provinceTex->data[address + 2] != (char)b) {
                                    continue;
                                }
                                if(map->centerTex->data[address] == 0) continue;

                                tx = x2;
                                ty = y2;

                                break;
                            }

                            if(tx != 0 || ty != 0) break;
                        }

                        if(tx != 0 || ty != 0) {
                            bool found = (selectedArmy == -1);

                            for(int i = 0; i < 4096; i++) {
                                if(selectedArmy == -1 && scenario->borderTex->data[address2] == (char)playerNation 
                                        && armies[i].x == tx && armies[i].y == ty) {
                                    selectedArmy = i;
                                    break;
                                
                                } else if(armies[i].x == tx && armies[i].y == ty && i == selectedArmy) {
                                    selectedArmy = -1;

                                } else if(scenario->borderTex->data[address2] == (char)playerNation && armies[i].x == tx && armies[i].y == ty) {
                                    armies[i].size += armies[selectedArmy].size;
                                    armies[selectedArmy].x = 0;
                                    armies[selectedArmy].y = 0;
                                    armies[selectedArmy].size = 0;
                                    selectedArmy = -1;
                                    found = true;
                                    break;

                                } else if(armies[i].x == tx && armies[i].y == ty && armies[i].size <= armies[selectedArmy].size) {
                                    armies[i].size = -armies[i].size + armies[selectedArmy].size;
                                    armies[selectedArmy].x = 0;
                                    armies[selectedArmy].y = 0;
                                    armies[selectedArmy].size = 0;

                                    selectedArmy = -1;
                                    found = true;
                                    nations[(unsigned char)scenario->borderTex->data[address2]].provinceCount--;
                                    nations[playerNation].provinceCount++;

                                    if(nations[(unsigned char)scenario->borderTex->data[address2]].provinceCount == 0) {
                                        for(int i = 0; i < 256; i++) {
                                            nations[(unsigned char)scenario->borderTex->data[address2]].wars[i] = false;
                                            nations[i].wars[(unsigned char)scenario->borderTex->data[address2]] = false;
                                        }

                                        if((char)selectedNation == scenario->borderTex->data[address2]) {
                                            selectedNation = 0;
                                        }

                                        address = (unsigned char)scenario->borderTex->data[address2] * scenario->colorTex->channels;

                                        scenario->colorTex->data[address] = 0;
                                        scenario->colorTex->data[address + 1] = 0;
                                        scenario->colorTex->data[address + 2] = 0;

                                        renderer_updateTexture(scenario->colorTex, true);
                                    }

                                    scenario->borderTex->data[address2] = (char)playerNation;
                                    renderer_updateTexture(scenario->borderTex, true);
                                    break;

                                } else if(armies[i].x == tx && armies[i].y == ty && armies[i].size > armies[selectedArmy].size) {
                                    armies[i].size -= armies[selectedArmy].size;
                                    armies[selectedArmy].x = 0;
                                    armies[selectedArmy].y = 0;
                                    armies[selectedArmy].size = 0;
                                    selectedArmy = -1;
                                    found = true;
                                    break;
                                }
                            }

                            if(!found) {
                                armies[selectedArmy].x = tx;
                                armies[selectedArmy].y = ty;
                                selectedArmy = -1;
                                nations[(unsigned char)scenario->borderTex->data[address2]].provinceCount--;
                                nations[playerNation].provinceCount++;

                                if(nations[(unsigned char)scenario->borderTex->data[address2]].provinceCount == 0) {
                                    for(int i = 0; i < 256; i++) {
                                        nations[(unsigned char)scenario->borderTex->data[address2]].wars[i] = false;
                                        nations[i].wars[(unsigned char)scenario->borderTex->data[address2]] = false;
                                    }

                                    if((char)selectedNation == scenario->borderTex->data[address2]) {
                                        selectedNation = 0;
                                    }

                                    address = (unsigned char)scenario->borderTex->data[address2] * scenario->colorTex->channels;

                                    scenario->colorTex->data[address] = 0;
                                    scenario->colorTex->data[address + 1] = 0;
                                    scenario->colorTex->data[address + 2] = 0;

                                    renderer_updateTexture(scenario->colorTex, true);
                                }

                                scenario->borderTex->data[address2] = (char)playerNation;
                                renderer_updateTexture(scenario->borderTex, true);
                            }
                        }
                    }

                    lastRightMouseButton = true;

                } else if(mode == 5) { // center editor
                    double x;
                    double y;

                    glfwGetCursorPos(window_window, &x, &y);

                    x = x / (float)window_width * 2 - 1;
                    y = 2 - y / (float)window_height * 2 - 1;
            
                    x = (x * camPosZ + camPosX) / 2.75f;
                    if(x < -1) x += 2;
                    if(x > 1) x -= 2;
                    y = y / aspect * camPosZ + camPosY;

                    int tx = (int)floor((x + 0.5) * (float)map->centerTex->width);
                    int ty = (int)floor((-y + 0.5) * (float)map->centerTex->height);

                    int address = (tx + map->centerTex->width * ty) * map->centerTex->channels;

                    map->centerTex->data[address] = 0;
                    map->centerTex->data[address + 1] = 0;
                    map->centerTex->data[address + 2] = 0;
                    renderer_updateTexture(map->centerTex, true);
                }
            }
        }

    } else {
        lastRightMouseButton = false;
    }

    if(glfwGetKey(window_window, GLFW_KEY_N) && !lastN) { // select new nation
        if(mode == 2) {
            selected = 1;

            while(!(scenario->colorTex->data[selected * scenario->colorTex->channels] == 0) ||
                    !(scenario->colorTex->data[selected * scenario->colorTex->channels + 1] == 0) ||
                    !(scenario->colorTex->data[selected * scenario->colorTex->channels + 2] == 0)) {
                selected++;
            }

            scenario->colorTex->data[selected * scenario->colorTex->channels] = rand() % 256;
            scenario->colorTex->data[selected * scenario->colorTex->channels + 1] = rand() % 256;
            scenario->colorTex->data[selected * scenario->colorTex->channels + 2] = rand() % 256;
            renderer_updateTexture(scenario->colorTex, true);
        }
    }
    lastN = glfwGetKey(window_window, GLFW_KEY_N);
}