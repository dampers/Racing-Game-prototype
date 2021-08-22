#include "stdafx.h"

void GAME_MAP::SetMap()
{
    memset(game_map, 0, sizeof(game_map));
    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            game_map[i][j] = ' ';
        }
    }
    int bound_x = 2 * GAME_SCREEN::WIDTH, bound_y = 2 * GAME_SCREEN::HEIGHT;
    for (int i = bound_x; i < WIDTH - bound_x; i++)
        game_map[bound_y][i] = game_map[HEIGHT - bound_y][i] = '*';
    for (int i = bound_y; i < HEIGHT - bound_y; i++)
        game_map[i][bound_x] = game_map[i][WIDTH - bound_x] = '*';

    int sq_x = WIDTH / 3, sq_y = HEIGHT / 4;
    for (int i = sq_y; i < sq_y + sq_y; i++)
    {
        for (int j = sq_x; j < sq_x + sq_x; j++)
        {
            game_map[i][j] = 'G';
        }
    }
    for (int i = GAME_SCREEN::WIDTH << 1; i < sq_x; i++)
        game_map[HEIGHT >> 1][i] = '-';
}

void GAME_MAP::GetMap(FILE* map_file)
{

}