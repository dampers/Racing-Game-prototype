#include "stdafx.h"

void INPUT_STREAM::driveInput()
{
    for (int i = 0; i < 6; i++)
        direction[i] = false;
    if (GetAsyncKeyState(VK_UP)) direction[UP] = true;
    if (GetAsyncKeyState(VK_DOWN)) direction[DOWN] = true;
    if (GetAsyncKeyState(VK_RIGHT)) direction[RIGHT] = true;
    if (GetAsyncKeyState(VK_LEFT)) direction[LEFT] = true;
    if (GetAsyncKeyState(0x41)) direction[DRIFT] = true; // 'A' key
}