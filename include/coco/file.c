#pragma once

#include <stdio.h>

void readFile(char* buf, char* path) {
    FILE* file = fopen(path, "rb");
    fread(buf, 1, 65536, file);
    fclose(file);
}