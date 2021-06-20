#include <iostream>
#include <windows.h>
#include <cmath>
#include <cstdio>

enum Direction { STOP = 0, UP, DOWN, RIGHT, LEFT };

static HANDLE screen[2];
int screen_index;

bool quit;
const double PI = 3.141592;
const int WIDTH = 700;
const int HEIGHT = 700;
const int CAMERAWIDTH = 80;
const int CAMERAHEIGHT = 25;
const double FPS = 60.0;
const double TPF = 1.0 / FPS;
const double MAX_SPEED = 5.0;

char screen_camera[(CAMERAHEIGHT+1)*(CAMERAWIDTH+1)];

double speed;
double dec = 0.3, acc = 0.2;
bool direction[5];
double angle;
double turn_speed = 0.08;

int pos_x, pos_y;
double vector_x, vector_y;
double dt = 1.0 / 10.0;
char game_map[1001][1001];
int camera_y, camera_x;
time_t old, cur;

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
	screen_index = screen_index?0:1;
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

void Setup()
{
	memset(screen_camera, '\0', sizeof(screen_camera));
	for (int i = CAMERAWIDTH; i < WIDTH-CAMERAWIDTH; i++)
	{
		game_map[CAMERAHEIGHT][i] = game_map[HEIGHT - CAMERAHEIGHT][i] = '|';
	}
	for (int i = CAMERAHEIGHT; i < HEIGHT-1; i++)
	{
		game_map[CAMERAHEIGHT][WIDTH - CAMERAWIDTH] = game_map[i][CAMERAWIDTH] = '|';
		for (int j = CAMERAWIDTH; j < WIDTH-CAMERAWIDTH; j++)
			game_map[i][j] = '.';
	}

// track


//
	ScreenInit();


	quit = false;

	speed = 0.0;
	pos_x = WIDTH / 2;
	pos_y = HEIGHT / 2;
	vector_x = vector_y = 0.0;

}

void Render()
{
	memset(screen_camera, '\0', sizeof(screen_camera));
	game_map[pos_y][pos_x] = 'o';
	game_map[pos_y][pos_x + 1] = 'o';
	game_map[pos_y + 1][pos_x] = 'O';
	game_map[pos_y + 1][pos_x + 1] = 'O';

	int screen_center_x = CAMERAWIDTH / 2;
	int screen_center_y = 2 * CAMERAHEIGHT / 3;
	int screen_pos = 0;
	int print_out_x = pos_x - screen_center_x;
	int print_out_y = pos_y - screen_center_y;

	for (int y = print_out_y; y < print_out_y + CAMERAHEIGHT; y++)
	{
		for (int x = print_out_x; x < print_out_x + CAMERAWIDTH; x++)
		{
			int center_relative_x = x - pos_x;
			int center_relative_y = y - pos_y;

			// 회전 행렬 계산
			double rotated_x = center_relative_x * cos(angle) - center_relative_y * sin(angle);
			double rotated_y = center_relative_x * sin(angle) + center_relative_y * cos(angle);
			int new_x = (int)rotated_x + pos_x;
			int new_y = (int)rotated_y + pos_y;
			screen_camera[(y - print_out_y) * (CAMERAWIDTH + 1) + (x - print_out_x)] = game_map[new_y][new_x];
			//screen_camera[screen_pos++] = game_map[new_y][new_x];
			//printf_s("%c ", game_map[new_y][new_x]);
		}
		screen_camera[(y - print_out_y) * (CAMERAWIDTH + 1) + CAMERAWIDTH] = '\n';
		//screen_camera[screen_pos++] = '\n';
		//printf_s("\n");
	}
	//screen_camera[screen_pos] = '\0';
	//screen_camera[CAMERAWIDTH * (CAMERAHEIGHT / 2) + CAMERAWIDTH / 2] = 'O';
	screen_camera[screen_pos] = '\0';

	game_map[pos_y][pos_x] = '#';
	game_map[pos_y][pos_x + 1] = '#';
	game_map[pos_y + 1][pos_x] = '#';
	game_map[pos_y + 1][pos_x + 1] = '#';

}

void Draw()
{
	ScreenClear();
	DWORD dw;
	SetConsoleCursorPosition(screen[screen_index], { 0, 0 });

	Render();

	WriteFile(screen[screen_index], screen_camera, sizeof(screen_camera), &dw, NULL);
	ScreenFlipping();

}
void Input()
{
	for (int i = 0; i < 5; i++)
	{
		direction[i] = false;
	}

	if (GetAsyncKeyState(VK_UP) & 0x8001) direction[UP] = true;
	if (GetAsyncKeyState(VK_DOWN) & 0x8001) direction[DOWN] = true;
	if (GetAsyncKeyState(VK_RIGHT) & 0x8001) direction[RIGHT] = true;
	if (GetAsyncKeyState(VK_LEFT) & 0x8001) direction[LEFT] = true;

	if (direction[UP] && speed < MAX_SPEED)
	{
		if (speed < 0) speed += dec;
		else speed += acc;
	}

	if (direction[DOWN] && speed > -MAX_SPEED)
	{
		if (speed > 0) speed -= dec;
		else speed -= acc;
	}

	// friction
	if (!direction[UP] && !direction[DOWN])
	{
		if (speed - dec > 0) speed -= dec;
		else if (speed + dec < 0) speed += dec;
		else speed = 0;
	}

	if (direction[RIGHT] && speed != 0.0) angle += turn_speed * speed / MAX_SPEED;
	if (direction[LEFT] && speed != 0.0) angle -= turn_speed * speed / MAX_SPEED;
}


void Logic()
{
	if (angle >= 2 * PI) angle -= 2 * PI;
	if (angle <= -2 * PI) angle += 2 * PI;
	pos_x += sin(angle) * speed;
	pos_y -= cos(angle) * speed;


	// bounds
	if (pos_x < CAMERAWIDTH+CAMERAWIDTH/5)
	{
		pos_x = CAMERAWIDTH+ CAMERAWIDTH / 5;
		speed = 0;
	}

	if (pos_x >= WIDTH - CAMERAWIDTH - 2)
	{
		pos_x = WIDTH - 5;
		speed = 0;
	}

	if (pos_y < CAMERAHEIGHT+CAMERAHEIGHT/5)
	{
		pos_y = CAMERAHEIGHT+ CAMERAHEIGHT / 5;
		speed = 0;
	}

	if (pos_y >= HEIGHT - CAMERAHEIGHT - 2)
	{
		pos_y = HEIGHT - 5;
		speed = 0;
	}
}


int main()
{

	Setup();
	while (!quit)
	{
		//Sleep(TPF);
		Sleep(1000/60);
		Input();
		Logic();
		Draw();
	}
	ScreenRelease();

	return 0;
}