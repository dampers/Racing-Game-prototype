#include <iostream>
#include <Windows.h>
#include <string.h>
#include <windows.graphics.h>
#pragma warning(disable:4996)


const int CAMERA_WIDTH = 80;
const int CAMERA_HEIGHT = 25;
const double MAX_SPEED = 3;
const int MAP_HEIGHT = 500;
const int MAP_WIDTH = 800;
const int SCREEN_HEIGHT = 25;
const int SCREEN_WIDTH = 80;
const double PI = 3.141592;

enum Direction { STOP = 0, UP, DOWN, RIGHT, LEFT, SHIFT };
bool direction[6] = {};

class Kart
{
    double speed = 0.0;
    double acc = 0.2;
    double dec = 0.3;
    double angle = 0.0;
    double turn_speed = 0.08;
    int pos_x;
    int pos_y;

    void move()
    {
        if (direction[UP] && speed < MAX_SPEED)
        {
            if (speed < 0) speed += dec; // deceleration
            else speed += acc; // acceleration
        }
        if (direction[DOWN] && speed > -MAX_SPEED)
        {
            if (speed > 0) speed -= dec; // deceleration
            else speed -= acc; // acceleration
        }
        // friction
        if (!direction[UP] && !direction[DOWN])
        {
            if (speed - dec > 0) speed -= dec;
            else if (speed + dec < 0) speed += dec;
            else speed = 0;
        }
        if (direction[LEFT] && speed != 0.0) angle -= turn_speed * speed / MAX_SPEED;
        if (direction[RIGHT] && speed != 0.0) angle += turn_speed * speed / MAX_SPEED;
        // drift
        if (direction[SHIFT])
        {
            if (direction[LEFT] && speed != 0.0) angle -= turn_speed * (MAX_SPEED - speed);
            if (direction[RIGHT] && speed != 0.0) angle += turn_speed * (MAX_SPEED - speed);

            if (speed - dec > 0) speed -= dec;
        }
        pos_x += sin(angle) * speed;
        pos_y -= cos(angle) * speed;

        if (angle >= 2 * PI) angle -= 2 * PI;
        if (angle <= -2 * PI) angle += 2 * PI;

        if (pos_x < CAMERA_WIDTH * 2)
        {
            speed = 0.0;
            pos_x = CAMERA_WIDTH * 2 + 1;
        }
        if (pos_x > MAP_WIDTH - 2 * CAMERA_WIDTH)
        {
            speed = 0.0;
            pos_x = MAP_WIDTH - 2 * CAMERA_WIDTH - 1;
        }
        if (pos_y < CAMERA_HEIGHT * 2)
        {
            speed = 0.0;
            pos_y = CAMERA_HEIGHT * 2 + 1;
        }
        if (pos_y > MAP_HEIGHT - 2 * CAMERA_HEIGHT)
        {
            speed = 0.0;
            pos_y = MAP_HEIGHT - 2 * CAMERA_HEIGHT - 1;
        }

        /*if (game_map[pos_y][pos_x] == 'G' || game_map[pos_y][pos_x] == 'Y')
        {
            speed = 1.0;
        }*/
    }

};

class Screen
{
    char screen_camera[CAMERA_HEIGHT + 1][CAMERA_WIDTH + 1];
    static HANDLE screen[2];
    int screen_index;

public:
    void screenInit()
    {
        CONSOLE_CURSOR_INFO cii;
        screen[0] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
        screen[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

        cii.dwSize = 1;
        cii.bVisible = FALSE;
        SetConsoleCursorInfo(screen[0], &cii);
        SetConsoleCursorInfo(screen[1], &cii);
    }
    void ScreenFlipping()
    {
        SetConsoleActiveScreenBuffer(screen[screen_index]);
        screen_index = screen_index ? 0 : 1;
    }
    void ScreenClear()
    {
        COORD Coor = { 0, 0 };
        DWORD dw;
        FillConsoleOutputCharacter(screen[screen_index], ' ', sizeof(screen_camera), Coor, &dw);
    }
    void ScreenRelease()
    {
        CloseHandle(screen[0]);
        CloseHandle(screen[1]);
    }
};

int main()
{
    std::cout << "Hello World!\n";
}

