#include <stdio.h>
#include <math.h>
#include <windows.h>

#define HEIGHT 25
#define WIDTH 60
#define GAME_TICK 10
#define WAVE_WIDTH 3
#define WAVE_FREQ 0.2
#define WAVE_INT 1.0
#define WAVE_DIR -1
#define WAVE_SPEED 1

#define ICON_WAVE '*'
#define ICON_EMPTY '.'

#define KEY_DIR_RIGHT 'd'
#define KEY_DIR_LEFT 'a'
#define KEY_INT_UP 'w'
#define KEY_INT_DOWN 's'
#define KEY_FREQ_UP 'e'
#define KEY_FREQ_DOWN 'q'
#define KEY_SPEED_UP 'r'
#define KEY_SPEED_DOWN 'f'
#define KEY_WIDTH_UP 't'
#define KEY_WIDTH_DOWN 'g'
#define KEY_ABORT 'x'


typedef struct {
	double width;
	double frequency;
	double intensity;
	int direction;
	double speed;
} Sinus_Data;


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


//(Re)Initializes the game_field with ICON_EMPTY characters
void reset_game_field(char* game_field) {

	for (int i = 0; i < HEIGHT; i++) {

		for (int j = 0; j < WIDTH; j++) {

			*(game_field + i * WIDTH + j) = ICON_EMPTY;
		}
	}
}


//Draws the game_field and current sinus_data to stdout
void draw_game_field(char* game_field, Sinus_Data sinus) {

	system("cls");

	for (int i = 0; i < HEIGHT; i++) {

		for (int j = 0; j < WIDTH; j++) {

			printf("%c", *(game_field + i * WIDTH + j));
		}

		printf("\n");
	}

	printf("\n");
	printf("Wave intensity:  %.2f     Press %c/%c to increase/decrease intensity.\n", sinus.intensity, KEY_INT_UP, KEY_INT_DOWN);
	printf("Wave frequency:  %.2f     Press %c/%c to increase/decrease frequency.\n", sinus.frequency, KEY_FREQ_UP, KEY_FREQ_DOWN);
	printf("Wave direction:  %s       Press %c/%c to set direction.\n", sinus.direction == 1 ? "<-" : "->", KEY_DIR_LEFT, KEY_DIR_RIGHT);
	printf("Wave speed:      %.2f     Press %c/%c to increase/decrease speed.\n", sinus.speed, KEY_SPEED_UP, KEY_SPEED_DOWN);
	printf("Wave width:      %.1f      Press %c/%c to increase/decrease width.\n", sinus.width, KEY_WIDTH_UP, KEY_WIDTH_DOWN);
	printf("Press %c to abort program.\n", KEY_ABORT);
}



void produce_sinus_wave(char* game_field, int step_count, Sinus_Data sinus) {

	double sin_value = 0.0;
	double wave_border = sinus.width * 0.1 / 2;


	for (int i = 0; i < WIDTH; i++) {

		for (int j = 0; j < HEIGHT; j++) {

			sin_value = sinus.intensity * sin(sinus.frequency * ((double)i + step_count));

			if (j / (HEIGHT - 1.0) * 2 - 1 <= sin_value + wave_border && j / (HEIGHT - 1.0) * 2 - 1 >= sin_value - wave_border)
				*(game_field + j * WIDTH + i) = ICON_WAVE;
			else *(game_field + j * WIDTH + i) = ICON_EMPTY;
		}
	}
}


/*
* Takes current input and changes the given sinus data accordingly.
* 
* @param sinus The given sinus data
* @return 0 by default
* @return 1 if player presses the abort key
*/
int get_input(Sinus_Data* sinus) {

	if (_kbhit()) {
		switch (getch()) {
		case KEY_DIR_LEFT:
			sinus->direction = 1;
			break;
		case KEY_DIR_RIGHT:
			sinus->direction = -1;
			break;
		case KEY_INT_UP:
			sinus->intensity += 0.05;
			break;
		case KEY_INT_DOWN:
			if (sinus->intensity > 0) sinus->intensity -= 0.05;
			break;
		case KEY_FREQ_UP:
			sinus->frequency += 0.02;
			break;
		case KEY_FREQ_DOWN:
			sinus->frequency -= 0.02;
			break;
		case KEY_SPEED_UP:
			if(sinus->speed < 1) sinus->speed += 0.05;
			break;
		case KEY_SPEED_DOWN:
			if(sinus->speed > 0) sinus->speed -= 0.05;
			break;
		case KEY_WIDTH_UP:
			sinus->width += 0.5;
			break;
		case KEY_WIDTH_DOWN:
			if(sinus->width > 0) sinus->width -= 0.5;
			break;
		case KEY_ABORT:
			return 1;
		}
	}

	return 0;
}


int main(int argc, char* argv[]) {

	char game_field[HEIGHT][WIDTH] = { 0 };
	Sinus_Data sinus = { WAVE_WIDTH, WAVE_FREQ, WAVE_INT, WAVE_DIR, WAVE_SPEED };
	int step_count = 0;
	double speed_count = 0;


	hide_cursor();
	reset_game_field(&game_field);

	while (1) {

		if (get_input(&sinus)) break;

		if (speed_count >= 1) {
			produce_sinus_wave(&game_field, step_count, sinus);
			step_count += sinus.direction;
			speed_count = 0;
		}
		else speed_count += sinus.speed;

		draw_game_field(&game_field, sinus);

		Sleep(GAME_TICK);
	}

	printf("\nPROGRAM ABORTED!\n");

	return 0;
}