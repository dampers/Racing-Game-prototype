#include "stdafx.h"


//FILE* fileInput = fopen("testInput.txt", "w");
//FILE* fileDrive = fopen("testLogic.txt", "w");
//FILE* fileDraw = fopen("testDraw.txt", "w");
//FILE* fileRender = fopen("testRender.txt", "w");
//FILE* fileCycle = fopen("testCycle.txt", "w");
//FILE* fileAngle = fopen("testAngle.txt", "w");

//time_t curr;

int main()
{

    // game set up
    static GAME_MAP GameMap;
    GameMap.SetMap();

    static GAME_SCREEN Screen;
    Screen.Init();

    static INPUT_STREAM InputStream;

    std::vector<KART> Kart_V;

    Kart_V.push_back(KART(GAME_MAP::WIDTH / 4, GAME_MAP::HEIGHT / 2 + 1, 0.0));

    time_t total_time = clock();
    /*time_t cycle_time = clock();*/
    while (true)
    {
        /*cycle_time = clock();*/

        Sleep(1);
        InputStream.driveInput();
        Kart_V[0].putDirection(InputStream.direction);
        Kart_V[0].drive();

        std::pair<int, int> KartCoord = Kart_V[0].getPosPair();
        Kart_V[0].checkMaterial(GameMap.game_map[KartCoord.second][KartCoord.first]);
        Screen.Draw(GameMap.game_map, KartCoord, Kart_V[0].getAngle(), Kart_V[0].getDrift());
        Screen.Render();

        /*fprintf(fileCycle, "%lf\n", ((double)clock()-cycle_time)/1000);
        fprintf(fileAngle, "%lf\n", Kart_V[0].getAngle());*/
    }

    Screen.Final();

    /*fclose(fileInput);
    fclose(fileDrive);
    fclose(fileDraw);
    fclose(fileRender);
    fclose(fileAngle);
    fclose(fileAngle);*/

    return 0;
}