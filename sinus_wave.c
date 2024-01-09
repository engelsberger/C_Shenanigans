#include <stdio.h>
#include <math.h>
#include <windows.h>

#define HEIGHT 25
#define WIDTH 60
#define GAME_TICK 100
#define WAVE_WIDTH 3
#define WAVE_FREQ 0.2

#define ICON_WAVE '*'
#define ICON_EMPTY '.'


/*
* Hides text cursor, prevents flickering (mostly)
* Requires <windows.h>
* Source: https://stackoverflow.com/questions/30126490/how-to-hide-console-cursor-in-c
*/
void hide_cursor()
{
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(consoleHandle, &info);
}


/*
* Initialises the given game_field with ICON_EMPTY
*/
void reset_game_field(char* game_field) {

	for (int i = 0; i < HEIGHT; i++) {

		for (int j = 0; j < WIDTH; j++) {

			*(game_field + i * WIDTH + j) = ICON_EMPTY;
		}
	}
}


/*
* Draws the given game_field to stdout
*/
void draw_game_field(char* game_field) {

	system("cls");

	for (int i = 0; i < HEIGHT; i++) {

		for (int j = 0; j < WIDTH; j++) {

			printf("%c", *(game_field + i * WIDTH + j));
		}

		printf("\n");
	}
}


/*
* Produces a customizable sinus wave and writes it into the game_field.
*/
void produce_sinus_wave(char* game_field, int step_count) {

	double sin_value = 0.0;
	double wave_border = WAVE_WIDTH * 0.1 / 2;


	for (int i = 0; i < WIDTH; i++) {

		for (int j = 0; j < HEIGHT; j++) {

			sin_value = sin(WAVE_FREQ * (i + step_count));

			if (j / (HEIGHT - 1.0) * 2 - 1 <= sin_value + wave_border && j / (HEIGHT - 1.0) * 2 - 1 >= sin_value - wave_border)
				*(game_field + j * WIDTH + i) = ICON_WAVE;
			else *(game_field + j * WIDTH + i) = ICON_EMPTY;
		}
	}
}


int main(int argc, char* argv[]) {

	char game_field[HEIGHT][WIDTH] = { 0 };
	int step_count = 0;
	int abort = 0;


	hide_cursor();
	reset_game_field(&game_field);

	while (!abort) {

		produce_sinus_wave(&game_field, step_count++);

		draw_game_field(&game_field);

		Sleep(GAME_TICK);
	}

	return 0;
}