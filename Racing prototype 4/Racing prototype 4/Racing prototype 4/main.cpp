﻿#include <iostream>
#include <Windows.h>
#include <string.h>
#include <vector>
#include <windows.graphics.h>

#pragma warning(disable:4996)
#define PI 3.1415926535

const int CAMERA_WIDTH = 80;
const int CAMERA_HEIGHT = 25;
const double MAX_SPEED = 3;
const int MAP_HEIGHT = 500;
const int MAP_WIDTH = 800;
const int SCREEN_HEIGHT = 25;
const int SCREEN_WIDTH = 80;

enum Direction { STOP = 0, UP, DOWN, RIGHT, LEFT, DRIFT };

class InputStream
{
public:
    bool direction[6];
    void driveInput()
    {
        for (int i = 0; i < 6; i++)
            direction[i] = false;
        if (GetAsyncKeyState(VK_UP)) direction[UP] = true;
        if (GetAsyncKeyState(VK_DOWN)) direction[DOWN] = true;
        if (GetAsyncKeyState(VK_RIGHT)) direction[RIGHT] = true;
        if (GetAsyncKeyState(VK_LEFT)) direction[LEFT] = true;
        if (GetAsyncKeyState(0x41)) direction[DRIFT] = true; // 'A' key
    }
};

class Kart
{
private:
    double speed = 0.0;
    double acc = 0.2;
    double dec = 0.3;
    double angle = 0.0;
    double turn_speed = 0.08;
    int pos_x;
    int pos_y;
    bool direction[6] = {};

public:

    Kart(int pos_x, int pos_y, double speed) : pos_x(pos_x), pos_y(pos_y), speed(speed) {}

    void putDirection(bool* direction)
    {
        for (int i = 0; i < 6; i++)
            this->direction[i] = direction[i];
    }

    int getAngle() { return angle; }
    
    void acceleration()
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
    }

    void friction()
    {
        if (!direction[UP] && !direction[DOWN])
        {
            if (speed - dec > 0) speed -= dec;
            else if (speed + dec < 0) speed += dec;
            else speed = 0;
        }
        if (direction[LEFT] && speed != 0.0) angle -= turn_speed * speed / MAX_SPEED;
        if (direction[RIGHT] && speed != 0.0) angle += turn_speed * speed / MAX_SPEED;
    }

    void drift()
    {
        if (direction[DRIFT])
        {
            if (direction[LEFT] && speed != 0.0) angle -= turn_speed * (MAX_SPEED - speed);
            if (direction[RIGHT] && speed != 0.0) angle += turn_speed * (MAX_SPEED - speed);
            if (speed - dec > 0) speed -= dec;
        }
    }

    void checkBoundary()
    {
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
    }

    void drive()
    {
        acceleration();
        friction();
        drift();

        pos_x += sin(angle) * speed;
        pos_y -= cos(angle) * speed;

        if (angle >= 2 * PI) angle -= 2 * PI;
        if (angle <= -2 * PI) angle += 2 * PI;

        checkBoundary();
    }

    void checkMaterial(char material)
    {
        if (material == 'G' || material == 'Y')
            speed = 1.0;
    }

};

class Screen
{
    static HANDLE screen[2];
    int screen_index;

public:
    char screen_camera[CAMERA_HEIGHT + 1][CAMERA_WIDTH + 1];
    
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


    void Render()
    {
        ScreenClear();
        DWORD screen_size = 0;
        SetConsoleCursorPosition(screen[screen_index], { 0, 0 });

        for (int screen_y = 0; screen_y <= SCREEN_HEIGHT; screen_y++)
        {
            for (int screen_x = 0; screen_x <= SCREEN_WIDTH; screen_x++)
            {
                if (screen_camera[screen_y][screen_x] == ' ')
                    SetConsoleTextAttribute(screen[screen_index], BACKGROUND_INTENSITY);

                else if (screen_camera[screen_y][screen_x] == 'G')
                    SetConsoleTextAttribute(screen[screen_index], FOREGROUND_GREEN | BACKGROUND_GREEN);

                else if (screen_camera[screen_y][screen_x] == 'o')
                    SetConsoleTextAttribute(screen[screen_index], FOREGROUND_RED | BACKGROUND_RED);

                else if (screen_camera[screen_y][screen_x] == '#')
                    SetConsoleTextAttribute(screen[screen_index], NULL);

                else if (screen_camera[screen_y][screen_x] == '*')
                    SetConsoleTextAttribute(screen[screen_index], FOREGROUND_BLUE | BACKGROUND_BLUE);

                else if (screen_camera[screen_y][screen_x] == 'Y')
                    SetConsoleTextAttribute(screen[screen_index], FOREGROUND_RED | FOREGROUND_GREEN | BACKGROUND_RED | BACKGROUND_GREEN);

                else if (screen_camera[screen_y][screen_x] == '-')
                    SetConsoleTextAttribute(screen[screen_index], FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);

                WriteFile(screen[screen_index], screen_camera[screen_y] + screen_x, 1, &screen_size, NULL);
            }
        }
        //WriteFile(screen[screen_index], screen_camera, sizeof(screen_camera), &dw, NULL);
        ScreenFlipping();
    }
};


class Map
{
    char game_map[1001][1001];

    void SetMap()
    {
        memset(game_map, 0, sizeof(game_map));
        for (int i = 0; i < MAP_HEIGHT; i++)
        {
            for (int j = 0; j < MAP_WIDTH; j++)
            {
                game_map[i][j] = ' ';
            }
        }
        int bound_x = 2 * CAMERA_WIDTH, bound_y = 2 * CAMERA_HEIGHT;
        for (int i = bound_x; i < MAP_WIDTH - bound_x; i++)
            game_map[bound_y][i] = game_map[MAP_HEIGHT - bound_y][i] = '*';
        for (int i = bound_y; i < MAP_HEIGHT - bound_y; i++)
            game_map[i][bound_x] = game_map[i][MAP_WIDTH - bound_x] = '*';

        int sq_x = MAP_WIDTH / 3, sq_y = MAP_HEIGHT / 4;
        for (int i = sq_y; i < sq_y + sq_y; i++)
        {
            for (int j = sq_x; j < sq_x + sq_x; j++)
            {
                game_map[i][j] = 'G';
            }
        }
        for (int i = CAMERA_WIDTH << 1; i < sq_x; i++)
            game_map[MAP_HEIGHT >> 1][i] = '-';
    }
    void GetMap(FILE* map_file)
    {

    }

};

int main()
{
    return 0;
}
