#include <stdio.h>
#include <windows.h>
#include "perlin_noise.h"


/*
* *** Game Settings ***
* 
* Quantity goes between 0 and 1, 0 is nothing and 1 is max
* QNTITY_GRASS		.. how much ground has grass
* QNTITY_TALL_GRASS .. how much grass is tall grass
* QNTITY_TREES		.. how much area is covered by trees
*/
#define WIDTH 100
#define HEIGHT 20
#define GAME_TICK 100
#define QNTITY_GRASS 0.6
#define QNTITY_TALL_GRASS 0.6
#define QNTITY_TREES 0.3
#define ICON_GROUND ' '
#define ICON_GRASS '.'
#define ICON_TALL_GRASS ':'
#define ICON_TREE 'Y'

/*
* *** Input Settings ***
*/
#define KEY_UP 'w'
#define KEY_DOWN 's'
#define KEY_LEFT 'a'
#define KEY_RIGHT 'd'
#define KEY_FREQ_UP 'u'
#define KEY_FREQ_DOWN 'j'
#define KEY_DEPTH_UP 'i'
#define KEY_DEPTH_DOWN 'k'
#define KEY_SEED_UP 'o'
#define KEY_SEED_DOWN 'l'
#define KEY_ABORT 'x'

/*
* *** Perlin Noise Default Settings ***
* 
* PROCED_FREQ	.. With changing frequency you can basically zoom in and out
* PROCED_DEPTH	.. With changing depth you change the level of detail
* PROCED_SEED	.. With the same seed you get the same pseudo random map
*/
#define PROCED_FREQ 0.1
#define PROCED_DEPTH 4
#define PROCED_SEED 1


typedef struct {
	int x;
	int y;
	float freq;
	int depth;
	int seed;
} Perlin_Data;

char game_field[HEIGHT][WIDTH];
int abort_program = 0;


// Sets the entire game field to ICON_GROUND
void reset_game_field() {

	for (int i = 0; i < HEIGHT - 1; i++) {

		for (int j = 0; j < WIDTH - 1; j++) {

			game_field[i][j] = ICON_GROUND;
		}
	}
}


/*
* Return a random number between a min and a max value, requires <windows.h>
*
* @param min The given min value
* @param max The given max value
* @return A random number between min and max
*/
int random(int min, int max) {

	return min + rand() / (RAND_MAX / (max - min + 1) + 1);
}


/*
* Generate a game field using given perlin values
* 
* @param perlin The given perlin data
*/
void generate_game_field(Perlin_Data perlin) {

	//Run through each individual spot in game_field
	for (int i = 0; i < HEIGHT - 1; i++) {

		for (int j = 0; j < WIDTH - 1; j++) {

			//Generate a perlin value according to the given data, to randomize trees more simply add 100 to the seed
			float perlin_value = perlin2d(j + perlin.x, i + perlin.y, perlin.freq, perlin.depth, perlin.seed);
			float perlin_value_trees = perlin2d(j + perlin.x, i + perlin.y, perlin.freq, perlin.depth, perlin.seed + 100);

			//Set the current spot accordingly
			if (perlin_value_trees <= QNTITY_TREES) {

				game_field[i][j] = ICON_TREE;
			}
			else if (perlin_value <= QNTITY_GRASS) {

				if (perlin_value <= QNTITY_GRASS * QNTITY_TALL_GRASS) game_field[i][j] = ICON_TALL_GRASS;
				else game_field[i][j] = ICON_GRASS;
			}
			else game_field[i][j] = ICON_GROUND;
		}
	}
}


//Draw the current game field into the console
void draw_game_field(Perlin_Data perlin) {

	system("cls");

	for (int i = 0; i <= HEIGHT; i++) {

		for (int j = 0; j <= WIDTH; j++) {

			if (j == 0 || j == WIDTH) printf("|");
			else if (i == 0 || i == HEIGHT) printf("-");
			else printf("%c", game_field[i - 1][j - 1]);
		}

		printf("\n");
	}

	printf("\nX-Offset: %d\nY-Offset: %d\nDistance: %.2f\nDetails:  %d\nSeed:     %d", 
		perlin.x, perlin.y, perlin.freq, perlin.depth, perlin.seed);

	printf("\nMove up/left/down/right with %c/%c/%c/%c\n", KEY_UP, KEY_LEFT, KEY_DOWN, KEY_RIGHT);
	printf("Increase/Decrease Distance with %c/%c\n", KEY_FREQ_UP, KEY_FREQ_DOWN);
	printf("Increase/Decrease Details with %c/%c\n", KEY_DEPTH_UP, KEY_DEPTH_DOWN);
	printf("Increase/Decrease Seed with %c/%c\n", KEY_SEED_UP, KEY_SEED_DOWN);
	printf("Press %c to end program\n", KEY_ABORT);
}


Perlin_Data get_input() {

	Perlin_Data input = { 0, 0, 0.0, 0, 0 };

	if (_kbhit()) {
		switch (getch()) {
			case KEY_UP:
				input.y = 1;
				break;
			case KEY_DOWN:
				input.y = -1;
				break;
			case KEY_RIGHT:
				input.x = -1;
				break;
			case KEY_LEFT:
				input.x = 1;
				break;
			case KEY_FREQ_UP:
				input.freq = 0.05;
				break;
			case KEY_FREQ_DOWN:
				input.freq = -0.05;
				break;
			case KEY_DEPTH_UP:
				input.depth = 1;
				break;
			case KEY_DEPTH_DOWN:
				input.depth = -1;
				break;
			case KEY_SEED_UP:
				input.seed = 1;
				break;
			case KEY_SEED_DOWN:
				input.seed = -1;
				break;
			case KEY_ABORT:
				abort_program = 1;
				break;
		}
	}

	return input;
}


int main(int argc, char* argv[]) {

	Perlin_Data current_perlin_data = { 0, 0, PROCED_FREQ, PROCED_DEPTH, PROCED_SEED };

	reset_game_field();

	generate_game_field(current_perlin_data);
	draw_game_field(current_perlin_data);

	while (!abort_program) {

		Perlin_Data input = get_input();

		if (input.x != 0 || input.y != 0 || input.freq != 0.0 || input.depth != 0 || input.seed != 0) {

			current_perlin_data.x += input.x;
			current_perlin_data.y += input.y;
			current_perlin_data.freq += input.freq;
			current_perlin_data.depth += input.depth;
			current_perlin_data.seed += input.seed;

			generate_game_field(current_perlin_data);
			draw_game_field(current_perlin_data);
		}

		Sleep(100);
	}

	draw_game_field(current_perlin_data);
	printf("\nAborted!\n");

	return 0;
}