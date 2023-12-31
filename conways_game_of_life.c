#include <stdio.h>
#include <Windows.h>

/*
* *** Game Settings ***
* 
* GAME_TICK		..The amount of milliseconds between each game field update
* SPAWN_CHANCE	..The chance for a cell to spawn on a field when setting up the game field(percentage, 0 - 100)
*/
#define WIDTH 60
#define HEIGHT 25
#define GAME_TICK 500
#define SPAWN_CHANCE 30
#define ICON_CELL '*'
#define ICON_GND '.'




/*
* Return a random number between a min and a max value
*
* @param min The given min value
* @param max The given max value
* @return A random number between min and max
*/
int random(int min, int max) {

	return min + rand() / (RAND_MAX / (max - min + 1) + 1);
}


/*
* Draw a given game_field on the console
* 
* @param game_field Pointer to the game field
* @param width Width of the given game field
* @param height Height of the given game field
*/
void draw_game_field(char* game_field, int width, int height) {

	system("cls");

	for (int i = 0; i < height; i++) {

		for (int j = 0; j < width; j++) {

			printf("%c", *(game_field + width * i + j));
		}

		printf("\n");
	}
}


/*
* Updates a given game field by one generation following the rules of conways game of life
* - Cells with one or less neighbours die
* - Cells with two or three neighbours live on
* - Cells with four or more neighbours die
* - Empty spaces with three neighbours create a new cell
* 
* @param game_field Pointer to the game field
* @param width Width of the given game field
* @param height Height of the given game field
* @return The number of cells alive
*/
int update_game_field(char* game_field, int width, int height) {

	//Define a new temporary array with a pointer to it
	char game_field_temp_arr[] = "";
	char* game_field_temp = &game_field_temp_arr;
	int neighbour_count = 0;
	int cell_count = 0;

	//Assign the required memory space for the new temporary array
	game_field_temp = (char*)malloc((width * height) * sizeof(char));

	//Run through the given game_field and set the temporary game field accordingly
	for (int i = 0; i < height; i++) {

		for (int j = 0; j < width; j++) {

			neighbour_count = 0;

			//Check all neighbours of the current position in game_field, each time another cell is detected increase neighbour_count
			for (int k = i - 1; k <= i + 1; k++) {

				for (int l = j - 1; l <= j + 1; l++) {

					if (!(k == i && l == j) && k >= 0 && l >= 0 && k < height && l < width &&
						*(game_field + width * k + l) == ICON_CELL) neighbour_count++;
				}
			}

			//Set the field in the temporary game field alive or dead, according to conways game of life rules (see function description)
			if (neighbour_count == 2) {

				*(game_field_temp + width * i + j) = *(game_field + width * i + j);
				if (*(game_field + width * i + j) == ICON_CELL) cell_count++;
			}
			else if (neighbour_count == 3) {

				*(game_field_temp + width * i + j) = ICON_CELL;
				cell_count++;
			}
			else *(game_field_temp + width * i + j) = ICON_GND;
		}
	}

	//Write the temporary game field to the actual game_field
	for (int i = 0; i < height; i++) {

		for (int j = 0; j < width; j++) {

			*(game_field + width * i + j) = *(game_field_temp + width * i + j);
		}
	}

	//Free the memory used for the temporary game field
	free(game_field_temp);

	return cell_count;
}


/*
* Fills a given game field with random cells according to a given spawn chance
*
* @param game_field Pointer to the game field
* @param width Width of the given game field
* @param height Height of the given game field
* @param spawn_chance The percentage of a cell to spawn on each field (0-100)
* @return The number of cells alive
*/
int reset_game_field(char* game_field, int width, int height, int spawn_chance) {

	int cell_count = 0;

	for (int i = 0; i < height; i++) {

		for (int j = 0; j < width; j++) {

			if (random(0, 99) < spawn_chance) {

				*(game_field + width * i + j) = ICON_CELL;
				cell_count++;
			}
			else *(game_field + width * i + j) = ICON_GND;
		}
	}

	return cell_count;
}


/*
* Hides text cursor for a better visual experience
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
	int cell_count = 0;
	int generation = 0;


	hide_cursor();

	cell_count = reset_game_field(game_field, WIDTH, HEIGHT, SPAWN_CHANCE);
	draw_game_field(game_field, WIDTH, HEIGHT);
	printf("\nGeneration %d: %d cells are alive.", generation, cell_count);
	Sleep(GAME_TICK);

	//Run through the loop as long as cell_count doesn't reach zero
	while (cell_count = update_game_field(game_field, WIDTH, HEIGHT)) {

		draw_game_field(game_field, WIDTH, HEIGHT);
		printf("\nGeneration %d: %d cells are alive.", ++generation, cell_count);

		Sleep(GAME_TICK);
	}

	draw_game_field(game_field, WIDTH, HEIGHT);
	printf("\nGeneration %d was the last surviving one.\n", generation);


	return 0;
}