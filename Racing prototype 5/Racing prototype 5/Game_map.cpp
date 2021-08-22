#include "stdafx.h"

void GAME_MAP::SetMap()
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

void GAME_MAP::GetMap(FILE* map_file)
{

}