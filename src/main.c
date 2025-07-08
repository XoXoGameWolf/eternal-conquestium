#include <coco/coco.h>
#include <time.h>

#include "game.c"
#include "init.c"
#include "update.c"
#include "render.c"

int main() {
    gameInit();

    while(window_open) {
        gameUpdate();
        gameRender();
    }

    //renderer_destroy();
    window_destroy();
    return 0;
}