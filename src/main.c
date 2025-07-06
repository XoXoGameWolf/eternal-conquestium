#include <coco/coco.h>

#include <time.h>

int main() {
    srand(time(NULL));

    window_init("Eternal Conquestium", 1600, 900);
    renderer_init(0.0f, 0.0f, 0.0f);

    window_fullscreen = true;

    Texture* mainMenuTex = renderer_createTexture("resources/ui/main_menu.bmp");
    Texture* provinceTex = renderer_createTexture("resources/map/provinces.bmp");
    Texture* terrainTex = renderer_createTexture("resources/map/terrain.bmp");
    Texture* borderTex = renderer_createTexture("resources/scenario/borders.bmp");
    Texture* colorTex = renderer_createTexture("resources/scenario/colors.bmp");
    Mesh* quad = renderer_createMeshFast(
        QUAD_VERTICES, sizeof(QUAD_VERTICES),
        QUAD_COORDS, sizeof(QUAD_COORDS),
        QUAD_INDICES, sizeof(QUAD_INDICES)
    );
    Shader* mapShader = renderer_createShader("resources/shaders/map/vert.glsl", "resources/shaders/map/frag.glsl");
    Shader* uiShader = renderer_createShader("resources/shaders/ui/vert.glsl", "resources/shaders/ui/frag.glsl");

    float camPosX = 0.0f;
    float camPosY = 0.0f;
    float camPosZ = 1.0f;
    float camVelX = 0.0f;
    float camVelY = 0.0f;
    float camVelZ = 0.0f;

    float aspect;

    int mode = 0;
    int selected = 1;

    bool lastF8 = false;
    bool lastLeftMouseButton = false;
    bool lastR = false;
    bool lastN = false;
    bool lastEscape = false;

    while(window_open) {
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

        if(glfwGetKey(window_window, GLFW_KEY_ESCAPE) && !lastEscape) {
            mode = 0;
        }
        lastEscape = glfwGetKey(window_window, GLFW_KEY_ESCAPE);

        if(glfwGetKey(window_window, GLFW_KEY_R) && !lastR) {
            renderer_deleteTexture(terrainTex);
            renderer_deleteTexture(borderTex);
            renderer_deleteTexture(colorTex);
            terrainTex = renderer_createTexture("resources/map/terrain.bmp");
            borderTex = renderer_createTexture("resources/scenario/borders.bmp");
            colorTex = renderer_createTexture("resources/scenario/colors.bmp");
        }
        lastR = glfwGetKey(window_window, GLFW_KEY_R);

        if(glfwGetKey(window_window, GLFW_KEY_N) && !lastN) {
            if(mode == 2) {
                selected = 1;

                while(!colorTex->data[selected * colorTex->channels] == 0 ||
                        !colorTex->data[selected * colorTex->channels + 1] == 0 ||
                        !colorTex->data[selected * colorTex->channels + 2] == 0) {
                    selected++;
                }

                colorTex->data[selected * colorTex->channels] = rand() % 256;
                colorTex->data[selected * colorTex->channels + 1] = rand() % 256;
                colorTex->data[selected * colorTex->channels + 2] = rand() % 256;
                renderer_updateTexture(colorTex);
            }
        }
        lastN = glfwGetKey(window_window, GLFW_KEY_N);

        if(glfwGetKey(window_window, GLFW_KEY_F8) && !lastF8) { // save
            renderer_saveTexture("resources/map/terrain.bmp", terrainTex);
            renderer_saveTexture("resources/scenario/borders.bmp", borderTex);
            renderer_saveTexture("resources/scenario/colors.bmp", colorTex);
        }
        lastF8 = glfwGetKey(window_window, GLFW_KEY_F8);

        if(glfwGetMouseButton(window_window, GLFW_MOUSE_BUTTON_LEFT)) { // set province
            double x;
            double y;

            glfwGetCursorPos(window_window, &x, &y);

            if(mode == 0 && !lastLeftMouseButton) {
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

            } else if((mode == 1 || mode == 2) && !lastLeftMouseButton) {
                x = x / window_width * 2 - 1;
                y = 2 - y / window_height * 2 - 1;

                x = (x * camPosZ + camPosX) / 2.75;
                if(x > 0.5) x -= 1;
                if(x < -0.5) x += 1;
                y = y / aspect * camPosZ + camPosY;

                if(x > -0.5f && x < 0.5f && y > -0.5f && y < 0.5f) {
                    int tx = (int)floor((x + 0.5) * provinceTex->width);
                    int ty = (int)floor((-y + 0.5) * provinceTex->height);

                    int address = (tx + ty * provinceTex->width) * provinceTex->channels;

                    unsigned char r = provinceTex->data[address];
                    unsigned char g = provinceTex->data[address + 1];
                    unsigned char b = provinceTex->data[address + 2];

                    tx = b + 256 * (g % 16);
                    ty = g / 16 + 16 * r;

                    if(mode == 1) {
                        address = (tx + ty * terrainTex->width) * terrainTex->channels;
                        
                        if(terrainTex->data[address] != selected) {
                            terrainTex->data[address] = selected;
                            terrainTex->data[address + 1] = 0;
                            terrainTex->data[address + 2] = 0;
                            renderer_updateTexture(terrainTex);
                        }

                    } else if(mode == 2) {
                        address = (tx + ty * borderTex->width) * borderTex->channels;
                        
                        if(borderTex->data[address] != selected) {
                            borderTex->data[address] = selected;
                            borderTex->data[address + 1] = 0;
                            borderTex->data[address + 2] = 0;
                            renderer_updateTexture(borderTex);
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
                int tx = (int)floor((x + 0.5) * provinceTex->width);
                int ty = (int)floor((-y + 0.5) * provinceTex->height);

                int address = (tx + ty * provinceTex->width) * provinceTex->channels;

                unsigned char r = provinceTex->data[address];
                unsigned char g = provinceTex->data[address + 1];
                unsigned char b = provinceTex->data[address + 2];

                tx = b + 256 * (g % 16);
                ty = g / 16 + 16 * r;

                if(mode == 1) {
                    address = (tx + ty * terrainTex->width) * terrainTex->channels;
                    selected = terrainTex->data[address];

                } else if(mode == 2) {
                    address = (tx + ty * borderTex->width) * borderTex->channels;
                    selected = borderTex->data[address];
                }
            }
        }

        camPosX += camVelX;
        camPosY += camVelY;
        camPosZ += camVelZ;

        camVelX *= 0.9f;
        camVelY *= 0.9f;
        camVelZ *= 0.9f;

        if(camPosZ > 0.5 * aspect) {
            camPosZ = 0.5 * aspect;
        }

        if(camPosY > 0.5 * ((0.5 * aspect) - camPosZ) / (0.5 * aspect)) {
            camPosY = 0.5 * ((0.5 * aspect) - camPosZ) / (0.5 * aspect);
        }

        if(camPosY < -0.5 * ((0.5 * aspect) - camPosZ) / (0.5 * aspect)) {
            camPosY = -0.5 * ((0.5 * aspect) - camPosZ) / (0.5 * aspect);
        }

        if(camPosX > 2.75f) {
            camPosX = -2.75f + (camPosX - 2.75f);
        }

        if(camPosX < -2.75f) {
            camPosX = 2.75f + (camPosX + 2.75f);
        }

        renderer_update();

        renderer_setUniformTexture(mapShader, "provinceTex", provinceTex, 0);
        renderer_setUniformTexture(mapShader, "terrainTex", terrainTex, 1);
        renderer_setUniformTexture(mapShader, "borderTex", borderTex, 2);
        renderer_setUniformTexture(mapShader, "colorTex", colorTex, 3);
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

    //renderer_destroy();
    window_destroy();
    return 0;
}