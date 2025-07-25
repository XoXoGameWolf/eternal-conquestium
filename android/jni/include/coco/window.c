#pragma once

#include <sys/time.h>
#include <math.h>

typedef struct {
    
} GLFWwindow;

#define 	GLFW_KEY_SPACE   32
#define 	GLFW_KEY_APOSTROPHE   39 /* ' */
#define 	GLFW_KEY_COMMA   44 /* , */
#define 	GLFW_KEY_MINUS   45 /* - */
#define 	GLFW_KEY_PERIOD   46 /* . */
#define 	GLFW_KEY_SLASH   47 /* / */
#define 	GLFW_KEY_0   48
#define 	GLFW_KEY_1   49
#define 	GLFW_KEY_2   50
#define 	GLFW_KEY_3   51
#define 	GLFW_KEY_4   52
#define 	GLFW_KEY_5   53
#define 	GLFW_KEY_6   54
#define 	GLFW_KEY_7   55
#define 	GLFW_KEY_8   56
#define 	GLFW_KEY_9   57
#define 	GLFW_KEY_SEMICOLON   59 /* ; */
#define 	GLFW_KEY_EQUAL   61 /* = */
#define 	GLFW_KEY_A   65
#define 	GLFW_KEY_B   66
#define 	GLFW_KEY_C   67
#define 	GLFW_KEY_D   68
#define 	GLFW_KEY_E   69
#define 	GLFW_KEY_F   70
#define 	GLFW_KEY_G   71
#define 	GLFW_KEY_H   72
#define 	GLFW_KEY_I   73
#define 	GLFW_KEY_J   74
#define 	GLFW_KEY_K   75
#define 	GLFW_KEY_L   76
#define 	GLFW_KEY_M   77
#define 	GLFW_KEY_N   78
#define 	GLFW_KEY_O   79
#define 	GLFW_KEY_P   80
#define 	GLFW_KEY_Q   81
#define 	GLFW_KEY_R   82
#define 	GLFW_KEY_S   83 
#define 	GLFW_KEY_T   84
#define 	GLFW_KEY_U   85
#define 	GLFW_KEY_V   86
#define 	GLFW_KEY_W   87
#define 	GLFW_KEY_X   88
#define 	GLFW_KEY_Y   89
#define 	GLFW_KEY_Z   90
#define 	GLFW_KEY_LEFT_BRACKET   91 /* [ */
#define 	GLFW_KEY_BACKSLASH   92 /* \ */
#define 	GLFW_KEY_RIGHT_BRACKET   93 /* ] */
#define 	GLFW_KEY_GRAVE_ACCENT   96 /* ` */
#define 	GLFW_KEY_WORLD_1   161 /* non-US #1 */
#define 	GLFW_KEY_WORLD_2   162 /* non-US #2 */
#define 	GLFW_KEY_ESCAPE   256
#define 	GLFW_KEY_ENTER   257
#define 	GLFW_KEY_TAB   258
#define 	GLFW_KEY_BACKSPACE   259
#define 	GLFW_KEY_INSERT   260
#define 	GLFW_KEY_DELETE   261
#define 	GLFW_KEY_RIGHT   262
#define 	GLFW_KEY_LEFT   263
#define 	GLFW_KEY_DOWN   264
#define 	GLFW_KEY_UP   265
#define 	GLFW_KEY_PAGE_UP   266
#define 	GLFW_KEY_PAGE_DOWN   267
#define 	GLFW_KEY_HOME   268
#define 	GLFW_KEY_END   269
#define 	GLFW_KEY_CAPS_LOCK   280
#define 	GLFW_KEY_SCROLL_LOCK   281
#define 	GLFW_KEY_NUM_LOCK   282
#define 	GLFW_KEY_PRINT_SCREEN   283
#define 	GLFW_KEY_PAUSE   284
#define 	GLFW_KEY_F1   290
#define 	GLFW_KEY_F2   291
#define 	GLFW_KEY_F3   292
#define 	GLFW_KEY_F4   293
#define 	GLFW_KEY_F5   294
#define 	GLFW_KEY_F6   295
#define 	GLFW_KEY_F7   296
#define 	GLFW_KEY_F8   297
#define 	GLFW_KEY_F9   298
#define 	GLFW_KEY_F10   299 
#define 	GLFW_KEY_F11   300
#define 	GLFW_KEY_F12   301
#define 	GLFW_KEY_F13   302
#define 	GLFW_KEY_F14   303
#define 	GLFW_KEY_F15   304
#define 	GLFW_KEY_F16   305
#define 	GLFW_KEY_F17   306
#define 	GLFW_KEY_F18   307
#define 	GLFW_KEY_F19   308
#define 	GLFW_KEY_F20   309
#define 	GLFW_KEY_F21   310
#define 	GLFW_KEY_F22   311
#define 	GLFW_KEY_F23   312
#define 	GLFW_KEY_F24   313 
#define 	GLFW_KEY_F25   314
#define 	GLFW_KEY_KP_0   320
#define 	GLFW_KEY_KP_1   321
#define 	GLFW_KEY_KP_2   322
#define 	GLFW_KEY_KP_3   323
#define 	GLFW_KEY_KP_4   324
#define 	GLFW_KEY_KP_5   325
#define 	GLFW_KEY_KP_6   326
#define 	GLFW_KEY_KP_7   327
#define 	GLFW_KEY_KP_8   328
#define 	GLFW_KEY_KP_9   329
#define 	GLFW_KEY_KP_DECIMAL   330
#define 	GLFW_KEY_KP_DIVIDE   331
#define 	GLFW_KEY_KP_MULTIPLY   332
#define 	GLFW_KEY_KP_SUBTRACT   333
#define 	GLFW_KEY_KP_ADD   334
#define 	GLFW_KEY_KP_ENTER   335
#define 	GLFW_KEY_KP_EQUAL   336
#define 	GLFW_KEY_LEFT_SHIFT   340
#define 	GLFW_KEY_LEFT_CONTROL   341
#define 	GLFW_KEY_LEFT_ALT   342
#define 	GLFW_KEY_LEFT_SUPER   343
#define 	GLFW_KEY_RIGHT_SHIFT   344
#define 	GLFW_KEY_RIGHT_CONTROL   345
#define 	GLFW_KEY_RIGHT_ALT   346
#define 	GLFW_KEY_RIGHT_SUPER   347
#define 	GLFW_KEY_MENU   348
#define 	GLFW_KEY_LAST   GLFW_KEY_MENU

#define 	GLFW_MOUSE_BUTTON_1   0
#define 	GLFW_MOUSE_BUTTON_2   1
#define 	GLFW_MOUSE_BUTTON_3   2
#define 	GLFW_MOUSE_BUTTON_4   3
#define 	GLFW_MOUSE_BUTTON_5   4
#define 	GLFW_MOUSE_BUTTON_6   5
#define 	GLFW_MOUSE_BUTTON_7   6
#define 	GLFW_MOUSE_BUTTON_8   7
#define 	GLFW_MOUSE_BUTTON_LAST   GLFW_MOUSE_BUTTON_8
#define 	GLFW_MOUSE_BUTTON_LEFT   GLFW_MOUSE_BUTTON_1
#define 	GLFW_MOUSE_BUTTON_RIGHT   GLFW_MOUSE_BUTTON_2
#define 	GLFW_MOUSE_BUTTON_MIDDLE   GLFW_MOUSE_BUTTON_3

double winMouseX;
double winMouseY;
double winMouseRadius;
double winLastMouseX;
double winLastMouseY;
double winLastMouseRadius;
int winMouseCount;
bool winMouseDown;

bool glfwGetKey(GLFWwindow* window, int key) {
    if(key == GLFW_KEY_A && winMouseCount == 2 && winMouseX > winLastMouseX) {
        return true;
    
    } else if(key == GLFW_KEY_D && winMouseCount == 2 && winMouseX < winLastMouseX) {
        return true;
    
    } else if(key == GLFW_KEY_S && winMouseCount == 2 && winMouseY < winLastMouseY) {
        return true;
    
    } else if(key == GLFW_KEY_W && winMouseCount == 2 && winMouseY > winLastMouseY) {
        return true;
    
    } else if(key == GLFW_KEY_E && winMouseCount == 2 && winLastMouseRadius < winMouseRadius) {
        return true;
    
    } else if(key == GLFW_KEY_Q && winMouseCount == 2 && winLastMouseRadius > winMouseRadius) {
        return true;

    } else {
        return false;
    }
}

bool glfwGetMouseButton(GLFWwindow* window, int button) {
    if(button == GLFW_MOUSE_BUTTON_LEFT) {
        return winMouseDown && winMouseCount == 1;

    } else {
        return false;
    }
}

void glfwGetCursorPos(GLFWwindow* window, double* x, double* y) {
    (*x) = winMouseX;
    (*y) = winMouseY;
}

double glfwGetTime() {
    struct timeval tv;
    gettimeofday(&tv, NULL);

    return (double)tv.tv_sec + (double)tv.tv_usec / 1000000;
}

GLFWwindow* window_window;

JNIEnv* env;
jobject res;

int window_width = 1280;
int window_height = 720;

bool window_open = true;
bool window_fullscreen = false;

void Java_org_coco_MainActivity_start(JNIEnv* _env, jclass cls, jobject _res) {
    env = _env;
    res = _res;

    start();

    if(!window_open) exit(0);
}

void Java_org_coco_MainActivity_update(JNIEnv* _env, jclass cls, jobject _res) {
    env = _env;
    res = _res;

    update();

    winLastMouseX = winMouseX;
    winLastMouseY = winMouseY;
    winLastMouseRadius = winMouseRadius;

    if(!window_open) exit(0);
}

void Java_org_coco_MainActivity_render(JNIEnv* _env, jclass cls, jobject _res) {
    env = _env;
    res = _res;

    render();

    if(!window_open) exit(0);
}

void Java_org_coco_MainActivity_size(JNIEnv* _env, jclass cls, jobject _res, jint width, jint height) {
    env = _env;
    res = _res;

    window_width = width;
    window_height = height;

    glViewport(0, 0, (int)width, (int)height);
}

void Java_org_coco_MainActivity_motion(JNIEnv* _env, jclass cls, jfloat x, jfloat y, jint action, jint count, jint id) {
    env = _env;

    winMouseCount = count;

    if(id == 0) {
        winMouseX = 0;
        winMouseY = 0;

    } else if(id == 1) {
        winMouseRadius = sqrt(pow(winMouseX * winMouseCount - (double)x, 2) + pow(winMouseY * winMouseCount - (double)y, 2));
    }

    winMouseX += (double)x / winMouseCount;
    winMouseY += (double)y / winMouseCount;

    if((int)action == 0x00000000) { // mouse down
        winMouseDown = true;

    } else if((int)action == 0x00000001) { // mouse up
        winMouseDown = false;
    }
}