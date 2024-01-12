#include <stdio.h>
#include <Windows.h>

#define HEIGHT 25
#define WIDTH 100
#define HEIGHT_GND (HEIGHT - 10)
#define GAME_TICK 5

#define ICON_AIR ' '
#define ICON_GND '.'
#define ICON_TURN '+'
#define ICON_STRAIGHT '*'


typedef struct {
	int x;
	int y;
} Vector2;


typedef struct {
	Vector2 pos;
	Vector2 dir;
} Ant;


/*
* Turns the given direction right or left
* 
* @param direction Pointer to the direction of an ant
* @param turn_right 1 if ant turns right, 0 if ant turns left
*/
void turn_ant(Vector2* direction, int turn_right) {

	int temp = 0;

	if (turn_right) {
		temp = direction->x;
		direction->x = direction->y;
		direction->y = -temp;
	}
	else {
		temp = direction->x;
		direction->x = -direction->y;
		direction->y = temp;
	}
}


/*
* Resets the game field, initializes it with ICON_GND up to a defined HEIGHT_GND,
* the rest gets initialized as ICON_AIR
*/
void reset_game_field(char* game_field) {

	for (int i = 0; i < HEIGHT; i++) {

		for (int j = 0; j < WIDTH; j++) {

			if (i < HEIGHT - HEIGHT_GND) *(game_field + i * WIDTH + j) = ICON_AIR;
			else *(game_field + i * WIDTH + j) = ICON_GND;
		}
	}
}


//Draws the game field
void draw_game_field(char* game_field) {

	system("cls");

	for (int i = 0; i < HEIGHT + 2; i++) {

		for (int j = 0; j < WIDTH + 2; j++) {

			if (j == 0 || j == WIDTH + 1) printf("|");
			else if (i == 0 || i == HEIGHT + 1) printf("-");
			else printf("%c", *(game_field + (i - 1) * WIDTH + (j - 1)));
		}
		printf("\n");
	}
}


/*
* Rules:
* - If the ant is on an ICON_GND field, it will turn right and turn this field into an ICON_TURN field
* - If the ant is on an ICON_TURN field, it will turn left and turn this field into an ICON_STRAIGHT field
* - If the ant is on an ICON_STRAIGHT field, it will go straight and turn this field into an ICON_GND field
* - If the ant is on an ICON_AIR field, it will turn around and turn this field into an ICON_GND field
* - If the ant hits the top/bottom of the field, it will turn right
*/
void move_ants(Ant* ants, int ant_count, char* game_field) {

	for (int i = 0; i < ant_count; i++) {

		switch (*(game_field + (ants + i)->pos.y * WIDTH + (ants + i)->pos.x)) {
		case ICON_GND:
			turn_ant(&((ants + i)->dir), 1);
			*(game_field + (ants + i)->pos.y * WIDTH + (ants + i)->pos.x) = ICON_TURN;
			break;
		case ICON_TURN:
			turn_ant(&((ants + i)->dir), 0);
			*(game_field + (ants + i)->pos.y * WIDTH + (ants + i)->pos.x) = ICON_STRAIGHT;
			break;
		case ICON_STRAIGHT:
			*(game_field + (ants + i)->pos.y * WIDTH + (ants + i)->pos.x) = ICON_GND;
			break;
		case ICON_AIR:
			(ants + i)->dir.x = -(ants + i)->dir.x;
			(ants + i)->dir.y = -(ants + i)->dir.y;
			*(game_field + (ants + i)->pos.y * WIDTH + (ants + i)->pos.x) = ICON_GND;
			break;
		}

		(ants + i)->pos.x += (ants + i)->dir.x;
		(ants + i)->pos.y += (ants + i)->dir.y;

		if ((ants + i)->pos.x < 0) (ants + i)->pos.x = WIDTH - 1;
		else if ((ants + i)->pos.x > WIDTH - 1) (ants + i)->pos.x = 0;

		if ((ants + i)->pos.y < 0 || (ants + i)->pos.y > HEIGHT - 1) {
			turn_ant(&((ants + i)->dir), 1);
		}
	}
}


int get_input() {

	if (_kbhit() && getch() == 'x') return 1;

	return 0;
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

	char game_field[HEIGHT][WIDTH] = { 0 };
	int draw_count = 0;

	Ant ants[] = {
		{ {WIDTH / 2 + 10, HEIGHT_GND / 2 + (HEIGHT - HEIGHT_GND)}, {0, 1}},
		{ {WIDTH / 2 + 5, HEIGHT_GND / 2 + (HEIGHT - HEIGHT_GND)}, {-1, 0} },
		{ {WIDTH / 2 - 5, HEIGHT_GND / 2 + (HEIGHT - HEIGHT_GND)}, {0, -1} },
		{ {WIDTH / 2 - 10, HEIGHT_GND / 2 + (HEIGHT - HEIGHT_GND)}, {1, 0} },
		{ {WIDTH / 2 + 10, HEIGHT_GND / 2 + (HEIGHT - HEIGHT_GND) - 4}, {-1, 0}},
		{ {WIDTH / 2 + 5, HEIGHT_GND / 2 + (HEIGHT - HEIGHT_GND) - 4}, {0, -1} },
		{ {WIDTH / 2 - 5, HEIGHT_GND / 2 + (HEIGHT - HEIGHT_GND) - 4}, {1, 0} },
		{ {WIDTH / 2 - 10, HEIGHT_GND / 2 + (HEIGHT - HEIGHT_GND) - 4}, {0, 1} }
	};

	reset_game_field(&game_field);
	hide_cursor();

	while (1) {

		if (get_input()) break;

		move_ants(&ants, sizeof(ants) / 16, &game_field);

		if (draw_count >= 10) {
			draw_game_field(&game_field);
			draw_count = 0;
		}
		else draw_count++;

		Sleep(GAME_TICK);
	}

	printf("\nABORTED!\n");
}