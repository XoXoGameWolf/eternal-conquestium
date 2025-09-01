#pragma once

void handleMotion() {
    aspect = (float)width / (float)height;

    if(getKey(KEY_W) && mode != 0 && mode != 6) { // move camera up
        cam_vel_y += 0.001f * cam_pos_z;
    }

    if(getKey(KEY_A) && mode != 0 && mode != 6) { // move camera left
        cam_vel_x -= 0.001f * cam_pos_z;
    }

    if(getKey(KEY_S) && mode != 0 && mode != 6) { // move camera down
        cam_vel_y -= 0.001f * cam_pos_z;
    }

    if(getKey(KEY_D) && mode != 0 && mode != 6) { // move camera right
        cam_vel_x += 0.001f * cam_pos_z;
    }

    if(getKey(KEY_E) && mode != 0 && mode != 6) { // zoom camera in
        cam_vel_z -= 0.002 * cam_pos_z;
    }

    if(getKey(KEY_Q) && mode != 0 && mode != 6) { // zoom camera out
        cam_vel_z += 0.002 * cam_pos_z;
    }

    // velocity
    cam_pos_x += cam_vel_x;
    cam_pos_y += cam_vel_y;
    cam_pos_z += cam_vel_z;

    // friction
    cam_vel_x *= 0.9f;
    cam_vel_y *= 0.9f;
    cam_vel_z *= 0.9f;

    // maximum zoom (whole map)
    if(cam_pos_z > 0.5 * aspect) {
        cam_pos_z = 0.5 * aspect;
    }

    // lock y below top of map
    if(cam_pos_y > 0.5 * ((0.5 * aspect) - cam_pos_z) / (0.5 * aspect)) {
        cam_pos_y = 0.5 * ((0.5 * aspect) - cam_pos_z) / (0.5 * aspect);
    }

    // lock y above bottom of map
    if(cam_pos_y < -0.5 * ((0.5 * aspect) - cam_pos_z) / (0.5 * aspect)) {
        cam_pos_y = -0.5 * ((0.5 * aspect) - cam_pos_z) / (0.5 * aspect);
    }

    // loop x when rightmost
    if(cam_pos_x > 2.75f) {
        cam_pos_x = -2.75f + (cam_pos_x - 2.75f);
    }

    // loop x when leftmost
    if(cam_pos_x < -2.75f) {
        cam_pos_x = 2.75f + (cam_pos_x + 2.75f);
    }
}

void update() {
    handleMotion();

    if(mode == 4 && getKey(KEY_R) && !lastR) {
        float x = getMouseX();
        float y = getMouseY();

        x = (x * cam_pos_z + cam_pos_x) / 2.75f;
        if(x > 1) x -= 2;
        if(x < -1) x += 2;
        y = y / aspect * cam_pos_z + cam_pos_y;

        x = x + 0.5;
        y = -y + 0.5;

        int tx = (int)(x * map->provinceTex->width);
        int ty = (int)(y * map->provinceTex->height);

        int address = (tx + map->provinceTex->width * ty) * map->provinceTex->channels;

        Color col = getPixel(map->provinceTex, tx, ty);

        tx = col.b + 256 * (col.g % 16);
        ty = col.g / 16 + 16 * col.r;

        address = (tx + scenario->borderTex->width * ty) * scenario->borderTex->channels;

        if(scenario->borderTex->data[address] == (char)playerNation) {
            int tx2 = 0;
            int ty2 = 0;

            for(tx = 0; tx < map->centerTex->width; tx++) {
                for(ty = 0; ty < map->centerTex->height; ty++) {
                    address = (tx + map->centerTex->width * ty) * map->centerTex->channels;

                    if(getPixel(map->centerTex, tx, ty).r == 0) continue;
                    if(getPixel(map->provinceTex, tx, ty).r != col.r || 
                        getPixel(map->provinceTex, tx, ty).g != col.g || 
                        getPixel(map->provinceTex, tx, ty).b != col.b) continue;

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
    lastR = getKey(KEY_R);

    if(getButton(BUTTON_LEFT)) { // set province
        float x = getMouseX();
        float y = getMouseY();

        if(mode == 5 && !lastLeftMouseButton) { // center editor
            int tx = (int)floor(((x + 1.0f) / 2.0f * width) / width * editorTex->width);
            int ty = (int)floor(((-y + 1.0f) / 2.0f * height) / height * editorTex->height);

            if(tx > 0 && tx < 291 && ty > 0 && ty < 90) {
                saveTexture("resources/map/terrain.bmp", map->terrainTex);
                saveTexture("resources/map/centers.bmp", map->centerTex);
                saveTexture("resources/scenario/borders.bmp", scenario->borderTex);
                saveTexture("resources/scenario/colors.bmp", scenario->colorTex);

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
                        nations[getPixel(scenario->borderTex, x, y).r].provinceCount++;
                    }
                }

            } else {
                x = (x * cam_pos_z + cam_pos_x) / 2.75f;
                if(x < -1) x += 2;
                if(x > 1) x -= 2;
                y = y / aspect * cam_pos_z + cam_pos_y;

                tx = (int)floor((x + 0.5) * (float)map->centerTex->width);
                ty = (int)floor((-y + 0.5) * (float)map->centerTex->height);

                setPixel(map->centerTex, tx, ty, (Color){255, 255, 255});
                updateTexture(map->centerTex, true);
            }

            lastLeftMouseButton = true;

        } else if(mode == 0 && !lastLeftMouseButton) { // main menu
            int tx = (int)floor(x / width * mainMenuTex->width);
            int ty = (int)floor(y / height * mainMenuTex->height);

            if(tx > 805 && tx < 1257 && ty > 325 && ty < 430) {
                mode = 3;
            }
            if(tx > 805 && tx < 1257 && ty > 448 && ty < 553) {
                mode = 6;
            }
            if(tx > 805 && tx < 1257 && ty > 567 && ty < 667) {
                open = false;
            }

            lastLeftMouseButton = true;
        
        } else if(mode == 4 && !lastLeftMouseButton) { // gameplay
            int tx = (int)floor(x / width * gameplayTex->width);
            int ty = (int)floor(y / height * gameplayTex->height);

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

                scenario->borderTex = createTexture("resources/scenario/borders.bmp", true);
                scenario->colorTex = createTexture("resources/scenario/colors.bmp", true);

                for(int x = 0; x < scenario->borderTex->width; x++) {
                    for(int y = 0; y < scenario->borderTex->height; y++) {
                        nations[getPixel(scenario->borderTex, x, y).r].provinceCount++;
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
                x = (x * cam_pos_z + cam_pos_x) / 2.75;
                if(x > 0.5) x -= 1;
                if(x < -0.5) x += 1;
                y = y / aspect * cam_pos_z + cam_pos_y;

                tx = (int)floor((x + 0.5) * map->provinceTex->width);
                ty = (int)floor((-y + 0.5) * map->provinceTex->height);

                Color col = getPixel(map->provinceTex, tx, ty);

                tx = col.b + 256 * (col.g % 16);
                ty = col.g / 16 + 16 * col.r;

                selectedNation = getPixel(scenario->borderTex, tx, ty).r;
                if(selectedNation == playerNation) selectedNation = 0;
            }

            lastLeftMouseButton = true;

        } else if(mode == 3 && !lastLeftMouseButton) { // nation selection
            int tx = (int)floor(x / width * nationSelectTex->width);
            int ty = (int)floor(y / height * nationSelectTex->height);

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
                x = (x * cam_pos_z + cam_pos_x) / 2.75;
                if(x > 0.5) x -= 1;
                if(x < -0.5) x += 1;
                y = y / aspect * cam_pos_z + cam_pos_y;
                
                tx = (int)floor((x + 0.5) * map->provinceTex->width);
                ty = (int)floor((-y + 0.5) * map->provinceTex->height);

                Color col = getPixel(map->provinceTex, tx, ty);

                tx = col.b + 256 * (col.g % 16);
                ty = col.g / 16 + 16 * col.r;

                if(getPixel(scenario->borderTex, tx, ty).r != 0) {
                    selectedNation = getPixel(scenario->borderTex, tx, ty).r;
                }
            }

            lastLeftMouseButton = true;

        } else if((mode == 1 || mode == 2) && !lastLeftMouseButton) { // terrain or border editor
            int tx = (int)floor(x / width * editorTex->width);
            int ty = (int)floor(y / height * editorTex->height);

            if(tx > 0 && tx < 291 && ty > 0 && ty < 90) {
                saveTexture("resources/map/terrain.bmp", map->terrainTex);
                saveTexture("resources/map/centers.bmp", map->centerTex);
                saveTexture("resources/scenario/borders.bmp", scenario->borderTex);
                saveTexture("resources/scenario/colors.bmp", scenario->colorTex);

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
                x = (x * cam_pos_z + cam_pos_x) / 2.75;
                if(x > 0.5) x -= 1;
                if(x < -0.5) x += 1;
                y = y / aspect * cam_pos_z + cam_pos_y;

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
                            updateTexture(map->terrainTex, true);

                            if(selected == 2) {
                                scenario->borderTex->data[address] = 0;
                                scenario->borderTex->data[address + 1] = 0;
                                scenario->borderTex->data[address + 2] = 0;
                                updateTexture(scenario->borderTex, true);
                            }
                        }
                        // border editor
                    } else if(mode == 2 && map->terrainTex->data[(tx + ty * map->terrainTex->width) * map->terrainTex->channels] != 2) {
                        address = (tx + ty * scenario->borderTex->width) * scenario->borderTex->channels;
                            
                        if(scenario->borderTex->data[address] != (char)selected) {
                            scenario->borderTex->data[address] = selected;
                            scenario->borderTex->data[address + 1] = 0;
                            scenario->borderTex->data[address + 2] = 0;
                            updateTexture(scenario->borderTex, true);
                        }
                    }
                }
            }

        } else if(mode == 6 && !lastLeftMouseButton) {
            int tx = (int)floor(x / width * editorSelectTex->width);
            int ty = (int)floor(y / height * editorSelectTex->height);

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

    if(getButton(BUTTON_RIGHT)) { // right click
        if(!lastRightMouseButton) {
            float x = getMouseX();
            float y = getMouseY();

            x = (x * cam_pos_z + cam_pos_x) / 2.75;
            if(x > 0.5) x -= 1;
            if(x < -0.5) x += 1;
            y = y / aspect * cam_pos_z + cam_pos_y;

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

                                        updateTexture(scenario->colorTex, true);
                                    }

                                    scenario->borderTex->data[address2] = (char)playerNation;
                                    updateTexture(scenario->borderTex, true);
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

                                    updateTexture(scenario->colorTex, true);
                                }

                                scenario->borderTex->data[address2] = (char)playerNation;
                                updateTexture(scenario->borderTex, true);
                            }
                        }
                    }

                    lastRightMouseButton = true;

                } else if(mode == 5) { // center editor
                    float x = getMouseX();
                    float y = getMouseY();
            
                    x = (x * cam_pos_z + cam_pos_x) / 2.75f;
                    if(x < -1) x += 2;
                    if(x > 1) x -= 2;
                    y = y / aspect * cam_pos_z + cam_pos_y;

                    int tx = (int)floor((x + 0.5) * (float)map->centerTex->width);
                    int ty = (int)floor((-y + 0.5) * (float)map->centerTex->height);

                    int address = (tx + map->centerTex->width * ty) * map->centerTex->channels;

                    map->centerTex->data[address] = 0;
                    map->centerTex->data[address + 1] = 0;
                    map->centerTex->data[address + 2] = 0;
                    updateTexture(map->centerTex, true);
                }
            }
        }

    } else {
        lastRightMouseButton = false;
    }

    if(getKey(KEY_N) && !lastN) { // select new nation
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
            updateTexture(scenario->colorTex, true);
        }
    }
    lastN = getKey(KEY_N);
}