#pragma once
#include <Windows.h>
#include <utility>
#include "const_define.h"

class GAME_SCREEN
{
    HANDLE screen[2];
    int screen_index;
    char screen_camera[CAMERA_HEIGHT + 1][CAMERA_WIDTH + 1];

    void ScreenFlipping();
    void ScreenClear();

public:
    void screenInit();
    void ScreenRelease();
    void Draw(char game_map[][MAX_WIDTH], std::pair<int, int> Kpos, double angle, bool drift);
    void Render();
};


