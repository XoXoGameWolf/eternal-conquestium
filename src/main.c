#include <coco/coco.h>

int main() {
    window_init("Eternal Conquestium", 1600, 900);
    renderer_init(0.2, 0.4f, 0.6f);

    window_fullscreen = true;

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

    float camPosX = 0.0f;
    float camPosY = 0.0f;
    float camPosZ = 1.0f;
    float camVelX = 0.0f;
    float camVelY = 0.0f;
    float camVelZ = 0.0f;

    bool lastLeftMouseButton = false;
    float aspect;

    int mode = 0;
    int selected = 1;

    bool lastF1 = false;
    bool lastF2 = false;
    bool lastF8 = false;

    while(window_open) {
        aspect = (float)window_width / (float)window_height;

        if(glfwGetKey(window_window, GLFW_KEY_W)) {
            camVelY += 0.001f * camPosZ;
        }
        if(glfwGetKey(window_window, GLFW_KEY_A)) {
            camVelX -= 0.001f * camPosZ;
        }
        if(glfwGetKey(window_window, GLFW_KEY_S)) {
            camVelY -= 0.001f * camPosZ;
        }
        if(glfwGetKey(window_window, GLFW_KEY_D)) {
            camVelX += 0.001f * camPosZ;
        }
        if(glfwGetKey(window_window, GLFW_KEY_E)) {
            camVelZ -= 0.002 * camPosZ;
        }
        if(glfwGetKey(window_window, GLFW_KEY_Q)) {
            camVelZ += 0.002 * camPosZ;
        }
        if(glfwGetKey(window_window, GLFW_KEY_F1) && !lastF1) {
            mode = 0;
        }
        lastF1 = glfwGetKey(window_window, GLFW_KEY_F1);

        if(glfwGetKey(window_window, GLFW_KEY_F2) && !lastF2) {
            mode = 1;
        }
        lastF2 = glfwGetKey(window_window, GLFW_KEY_F2);

        if(glfwGetKey(window_window, GLFW_KEY_F8) && !lastF8) {
            renderer_saveTexture("resources/map/terrain.bmp", terrainTex);
            renderer_saveTexture("resources/scenario/borders.bmp", borderTex);
        }
        lastF8 = glfwGetKey(window_window, GLFW_KEY_F8);

        if(glfwGetMouseButton(window_window, GLFW_MOUSE_BUTTON_LEFT)) {
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

                if(mode == 0) {
                    address = (tx + ty * terrainTex->width) * terrainTex->channels;
                    
                    if(terrainTex->data[address] != selected) {
                        terrainTex->data[address] = selected;
                        terrainTex->data[address + 1] = 0;
                        terrainTex->data[address + 2] = 0;
                        renderer_updateTexture(terrainTex);
                    }

                } else if(mode == 1) {
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

        if(glfwGetMouseButton(window_window, GLFW_MOUSE_BUTTON_RIGHT)) {
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

                if(mode == 0) {
                    address = (tx + ty * terrainTex->width) * terrainTex->channels;
                    selected = terrainTex->data[address];

                } else if(mode == 1) {
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

        window_update();
    }

    renderer_destroy();
    window_destroy();
    return 0;
}