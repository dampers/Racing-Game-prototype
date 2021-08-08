#include <iostream>
#include <Windows.h>
#include <string.h>
#include <windows.graphics.h>

#pragma warning(disable:4996)

char game_map[1001][1001];
const int MAP_HEIGHT = 500;
const int MAP_WIDTH = 800;
const int SCREEN_HEIGHT = 25;
const int SCREEN_WIDTH = 80;
const double PI = 3.141592;


enum Direction { STOP = 0, UP, DOWN, RIGHT, LEFT, SHIFT };
bool direction[6] = {};
int pos_x;
int pos_y;


const double MAX_SPEED = 3;
double speed = 0.0;
double acc = 0.2;
double dec = 0.3;
double angle = 0.0;
double turn_speed = 0.08;

double SLIP_INIT = 5 * PI;
double slip_angle = 5 * PI;

time_t total_time;

static HANDLE screen[2];
int screen_index;
const int CAMERAWIDTH = 80;
const int CAMERAHEIGHT = 25;
//char screen_camera[(CAMERAHEIGHT + 1) * (CAMERAWIDTH + 1) + 25];
char screen_camera[CAMERAHEIGHT + 1][CAMERAWIDTH + 1];

void ScreenInit()
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


void Input()
{
	for (int i = 0; i < 6; i++)
		direction[i] = false;
	if (GetAsyncKeyState(VK_UP)) direction[UP] = true;
	if (GetAsyncKeyState(VK_DOWN)) direction[DOWN] = true;
	if (GetAsyncKeyState(VK_RIGHT)) direction[RIGHT] = true;
	if (GetAsyncKeyState(VK_LEFT)) direction[LEFT] = true;
	if (GetAsyncKeyState(0x41)) direction[SHIFT] = true; // 'A' key
}

void Logic()
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

		//if (SLIP_INIT == slip_angle) slip_angle = angle;

		/*if (direction[LEFT] && speed != 0.0) slip_angle -= turn_speed * speed / MAX_SPEED;
		if (direction[RIGHT] && speed != 0.0) slip_angle += turn_speed * speed / MAX_SPEED;

		if (speed - dec > 0) speed -= dec;

		pos_x += sin(slip_angle) * speed;
		pos_y -= cos(slip_angle) * speed;*/

		if (direction[LEFT] && speed != 0.0) angle -= turn_speed * (MAX_SPEED-speed);
		if (direction[RIGHT] && speed != 0.0) angle += turn_speed * (MAX_SPEED-speed);

		if (speed - dec > 0) speed -= dec;
	}

	pos_x += sin(angle) * speed;
	pos_y -= cos(angle) * speed;

	if (angle >= 2 * PI) angle -= 2 * PI;
	if (angle <= -2 * PI) angle += 2 * PI;



	// bound
	if (pos_x < CAMERAWIDTH * 2)
	{
		speed = 0.0;
		pos_x = CAMERAWIDTH * 2 + 1;
	}
	if (pos_x > MAP_WIDTH - 2 * CAMERAWIDTH)
	{
		speed = 0.0;
		pos_x = MAP_WIDTH - 2 * CAMERAWIDTH - 1;
	}
	if (pos_y < CAMERAHEIGHT * 2)
	{
		speed = 0.0;
		pos_y = CAMERAHEIGHT * 2 + 1;
	}
	if (pos_y > MAP_HEIGHT - 2 * CAMERAHEIGHT)
	{
		speed = 0.0;
		pos_y = MAP_HEIGHT - 2 * CAMERAHEIGHT - 1;
	}

	if (game_map[pos_y][pos_x] == 'G' || game_map[pos_y][pos_x] == 'Y')
	{
		speed = 1.0;
	}
}

void Render()
{
	memset(screen_camera, 0, sizeof(screen_camera));
	char tmp = game_map[pos_y][pos_x];
	game_map[pos_y][pos_x] = 'o';
	int screen_center_x = CAMERAWIDTH / 2;
	int screen_center_y = 3 * CAMERAHEIGHT / 4;
	int screen_pos = 0;
	int print_out_x = pos_x - screen_center_x > 0 ? pos_x - screen_center_x : 0;
	int print_out_y = pos_y - screen_center_y > 0 ? pos_y - screen_center_y : 0;

	for (int y = print_out_y; y < print_out_y + CAMERAHEIGHT; y++)
	{
		for (int x = print_out_x; x < print_out_x + CAMERAWIDTH; x++)
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
		screen_camera[y - print_out_y][CAMERAWIDTH] = '\n';
		//screen_camera[(y - print_out_y) * (CAMERAWIDTH + 1) + CAMERAWIDTH] = '\n';
	}

	if (tmp == 'G') tmp = 'Y';
	game_map[pos_y][pos_x] = tmp;
	if (direction[SHIFT]) game_map[pos_y][pos_x] = '#';

	time_t next_time = clock();
	int min = (next_time / 1000) / 60;
	int sec = (next_time / 1000) % 60;
	int msec = next_time % 1000;
	sprintf(screen_camera[CAMERAHEIGHT], "%2d : %2d : %d\n", min, sec, msec);
}

void Draw()
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

void Setup()
{
	total_time = clock();

	memset(game_map, 0, sizeof(game_map));

	for (int i = 0; i < MAP_HEIGHT; i++)
	{
		for (int j = 0; j < MAP_WIDTH; j++)
		{
			game_map[i][j] = ' ';
		}
	}

	int bound_x = 2 * CAMERAWIDTH, bound_y = 2 * CAMERAHEIGHT;
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

	pos_y = MAP_HEIGHT / 2 + 1;
	pos_x = MAP_WIDTH / 4;

	for (int i = CAMERAWIDTH << 1; i < sq_x; i++)
		game_map[MAP_HEIGHT >> 1][i] = '-';

	speed = 0.0;

	ScreenInit();

}

int main()
{
	Setup();

	while (true)
	{
		Sleep(1);
		Input();
		Logic();
		Render();
		Draw();
	}

	ScreenRelease();

	return 0;
}