#include "stdafx.h"

void GAME_SCREEN::FlipBuffer()
{
    SetConsoleActiveScreenBuffer(screen[screen_index]);
    screen_index = screen_index ? 0 : 1;
}

void GAME_SCREEN::Clear()
{
    COORD Coor = { 0, 0 };
    DWORD dw;
    FillConsoleOutputCharacter(screen[screen_index], ' ', sizeof(screen_camera), Coor, &dw);
}

void GAME_SCREEN::Init()
{
    CONSOLE_CURSOR_INFO cii;
    screen[0] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    screen[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

    cii.dwSize = 1;
    cii.bVisible = FALSE;
    SetConsoleCursorInfo(screen[0], &cii);
    SetConsoleCursorInfo(screen[1], &cii);
}

void GAME_SCREEN::Final()
{
    CloseHandle(screen[0]);
    CloseHandle(screen[1]);
}

void GAME_SCREEN::Draw(char game_map[][GAME_MAP::MAX_WIDTH], std::pair<int, int> Kpos, double angle, bool drift)
{
    /*curr = clock();*/

    int pos_y = Kpos.second, pos_x = Kpos.first;
    memset(screen_camera, 0, sizeof(screen_camera));
    char tmp = game_map[pos_y][pos_x];
    game_map[pos_y][pos_x] = 'o';
    int screen_center_x = GAME_SCREEN::WIDTH / 2;
    int screen_center_y = 3 * GAME_SCREEN::HEIGHT / 4;
    int screen_pos = 0;
    int print_out_x = pos_x - screen_center_x > 0 ? pos_x - screen_center_x : 0;
    int print_out_y = pos_y - screen_center_y > 0 ? pos_y - screen_center_y : 0;

    for (int y = print_out_y; y < print_out_y + GAME_SCREEN::HEIGHT; y++)
    {
        for (int x = print_out_x; x < print_out_x + GAME_SCREEN::WIDTH; x++)
        {
            int center_relative_x = x - pos_x;
            int center_relative_y = y - pos_y;

            double rotated_x = center_relative_x * cos(angle) - center_relative_y * sin(angle);
            double rotated_y = center_relative_x * sin(angle) + center_relative_y * cos(angle);
            int new_x = (int)rotated_x + pos_x;
            int new_y = (int)rotated_y + pos_y;
            screen_camera[y - print_out_y][x - print_out_x] = game_map[new_y][new_x];
            //screen_camera[(y - print_out_y) * (CAMERAWIDTH + 1) + (x - print_out_x)] = game_map[new_y][new_x];
        }
        screen_camera[y - print_out_y][GAME_SCREEN::WIDTH] = '\n';
        //screen_camera[(y - print_out_y) * (CAMERAWIDTH + 1) + CAMERAWIDTH] = '\n';
    }

    if (tmp == 'G') tmp = 'Y';
    game_map[pos_y][pos_x] = tmp;
    if (drift) game_map[pos_y][pos_x] = '#';

    time_t next_time = clock();
    int min = (next_time / 1000) / 60;
    int sec = (next_time / 1000) % 60;
    int msec = next_time % 1000;
    sprintf(screen_camera[GAME_SCREEN::HEIGHT], "%2d : %2d : %d\n", min, sec, msec);

    /*fprintf(fileDraw, "%lf\n", ((double)clock() - curr) / 1000);*/
}

void GAME_SCREEN::Render()
{
    /*curr = clock();*/

    Clear();
    DWORD screen_size = 0;
    SetConsoleCursorPosition(screen[screen_index], { 0, 0 });

    for (int screen_y = 0; screen_y <= HEIGHT; screen_y++)
    {
        for (int screen_x = 0; screen_x <= WIDTH; screen_x++)
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

            else if (screen_camera[screen_y][screen_x] == '!')
                SetConsoleTextAttribute(screen[screen_index], FOREGROUND_BLUE);
            WriteFile(screen[screen_index], screen_camera[screen_y] + screen_x, 1, &screen_size, NULL);
        }
    }
    //WriteFile(screen[screen_index], screen_camera, sizeof(screen_camera), &dw, NULL);
    FlipBuffer();

    /*fprintf(fileRender, "%lf\n", ((double)clock() - curr) / 1000);*/
}