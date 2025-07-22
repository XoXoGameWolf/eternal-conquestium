#pragma once

#include <stdbool.h>
#include <stdlib.h>

#include <jni.h>
#include <android/log.h>
#include <GLES3/gl32.h>

void start();
void update();
void render();

#include <coco/window.c>
#include <coco/renderer.c>