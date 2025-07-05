#pragma once

#include <stdio.h>

void readFile(char* buf, char* path) {
    FILE* file = fopen(path, "rb");
    fread(buf, 1, 4096, file);
    fclose(file);
}