#pragma once
#include <Windows.h>
#include <utility>
#include "const_define.h"
#include "Game_map.h"

class GAME_SCREEN
{
public:
    static const int HEIGHT = 25;
    static const int WIDTH = 80;

    void Init();
    void Final();
    void Draw(char game_map[][GAME_MAP::MAX_WIDTH], std::pair<int, int> Kpos, double angle, bool drift);
    void Render();

private:
    HANDLE screen[2];
    int screen_index;
    char screen_camera[HEIGHT + 1][WIDTH + 1];

    void FlipBuffer();
    void Clear();
};


