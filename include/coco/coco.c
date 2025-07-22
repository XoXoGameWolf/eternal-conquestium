#pragma once

#include <coco/window.c>
#include <coco/renderer.c>

void start();
void update();
void render();

int main() {
    window_init(title, 1280, 720);
    start();

    while(window_open) {
        update();
        render();
        window_update();
    }

    window_destroy();
}