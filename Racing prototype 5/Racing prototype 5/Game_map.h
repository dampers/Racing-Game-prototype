#pragma once
#include "const_define.h"
#include <iostream>

class GAME_MAP
{
public:
    char game_map[MAX_HEIGHT][MAX_WIDTH];
    void SetMap();
    void GetMap(FILE* map_file);
};
