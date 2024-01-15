#include <stdio.h>
#include <Windows.h>


#define MAX_ITERATIONS 1000000

#define HEIGHT 25
#define WIDTH 61
#define GAME_TICK 500

#define ICON_ZERO ' '
#define ICON_QUARTER '.'
#define ICON_HALF '+'
#define ICON_FULL '#'


double changer_function(double input) {

	return (1.5 * input <= 1.0) ? 1.5 * input : 0.5 * input;
	
	//return (input <= 1.0) ? 1.5 * input : 0.5 * input;
}


/*
* Each field in game_field is calculated with the average value of all its neighbours run through the changer_function
*/
void update_game_field(double* game_field) {

	double* updated_game_field = (double*)malloc(HEIGHT * WIDTH * sizeof(double));

	for (int i = 0; i < HEIGHT; i++) {

		for (int j = 0; j < WIDTH; j++) {

			double sum = 0;
			int count = 0;

			for (int k = i - 1; k <= i + 1; k++) {

				for (int l = j - 1; l <= j + 1; l++) {

					/*Test 1: Average between all neighbours
					*/
					if (k >= 0 && k < HEIGHT && l >= 0 && l < WIDTH) {

						sum += *(game_field + k * WIDTH + l);
						count++;
					}

					/*Test 2: Shift average right
					*
					if (k >= 0 && k < HEIGHT && l >= 0 && l < WIDTH) {

						sum += (l < j) ? *(game_field + k * WIDTH + l) / 2 : (l == j) ? *(game_field + k * WIDTH + l) : *(game_field + k * WIDTH + l) * 2;
						count++;
					}*/
				}
			}

			*(updated_game_field + i * WIDTH + j) = count > 0 ? changer_function(sum / count) : 0.0;
		}
	}

	for (int i = 0; i < HEIGHT; i++) {

		for (int j = 0; j < WIDTH; j++) {

			*(game_field + i * WIDTH + j) = *(updated_game_field + i * WIDTH + j);
		}
	}

	free(updated_game_field);
}


void draw_game_field(double* game_field) {

	system("cls");

	for (int i = 0; i < HEIGHT + 2; i++) {

		for (int j = 0; j < WIDTH + 2; j++) {

			if (j == 0 || j == WIDTH + 1) printf("|");
			else if (i == 0 || i == HEIGHT + 1) printf("-");
			else if (*(game_field + (i - 1) * WIDTH + j - 1) < 0.25) printf("%c", ICON_ZERO);
			else if (*(game_field + (i - 1) * WIDTH + j - 1) < 0.5) printf("%c", ICON_QUARTER);
			else if (*(game_field + (i - 1) * WIDTH + j - 1) < 0.75) printf("%c", ICON_HALF);
			else printf("%c", ICON_FULL);
		}
		printf("\n");
	}
}


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


int main(int argc, char* argv[]) {

	double game_field[HEIGHT][WIDTH] = { 0 };


	hide_cursor();

	game_field[HEIGHT / 2][WIDTH / 2] = 1.0;

	draw_game_field(&game_field);
	Sleep(GAME_TICK);

	for (int i = 0; i < MAX_ITERATIONS; i++) {

		update_game_field(&game_field);

		draw_game_field(&game_field);

		Sleep(GAME_TICK);
	}

	return 0;
}