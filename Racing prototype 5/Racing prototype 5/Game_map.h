#pragma once
#include "const_define.h"
#include <iostream>

class GAME_MAP
{
public:
    static const int MAX_HEIGHT = 1001;
    static const int MAX_WIDTH = 1001;
    static const int HEIGHT = 500;
    static const int WIDTH = 800;

    char game_map[MAX_HEIGHT][MAX_WIDTH];
    void SetMap();
    void GetMap(FILE* map_file);
};
