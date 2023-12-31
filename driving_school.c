#include<stdio.h>
#include <windows.h>

/*
* *** Game Settings ***
* 
* ROWS			The amount of rows the car can drive on
* TRUE_ROWS		The actual amount of rows existing in the game field
* LENGTH		The length of the game field in columns
* GAME_TICK		The amount of milliseconds between game ticks
* OBSTCLE_SPD_TICK		how many GAME_TICKs it takes to move the obstacles
* OBSTCLE_SPAWN_TICK	how many OBSTCLE_SPD_TICK it takes to spawn another obstacle
* OBSTLCE_SPD_INCR_VAL	after what score the obstacle speed gets increased (value 100 means after every 100 points speed increases)
*/
#define ROWS 3
#define TRUE_ROWS (3 + (((ROWS) - 1) * 2))
#define LENGTH 30
#define GAME_TICK 20
#define OBSTCLE_SPD_TICK 4
#define OBSTCLE_SPAWN_TICK 5
#define OBSTLCE_SPD_INCR_VAL 20
#define BTN_UP 'w'
#define BTN_DOWN 's'
#define BTN_FORWARD 'd'
#define BTN_BACKWARD 'a'
#define BTN_QUIT 'x'

/*
* *** Player Settings ***
* 
* PLYR_STRT_ROW		goes from 1 to ROWS, defines start row for the player
* TRUE_STRT_ROW		is the actual y coordinates of the player start row
* PLYR_STRT_COL		goes from 1 to LENGTH-1, defines the start column for the player
* PLYR_LEN			minimum 2, player is two "8" with "-" between them (length 4 looks like "8--8")
*/
#define PLYR_STRT_ROW 1
#define TRUE_STRT_ROW ((PLYR_STRT_ROW) * 2 - 1)
#define PLYR_STRT_COL 3
#define PLYR_LEN 3

/*
* *** Graphics Settings ***
*/
#define ICON_TIRE '8'
#define ICON_CAR '-'
#define ICON_OBSTCL '#'
#define ICON_STREET '.'



typedef struct {
	int y;
	int x;
} Vector2;

char game_field[TRUE_ROWS][LENGTH];
Vector2 player_coordinates;
int game_over = 0;
long long int score = 0;


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
* Checks if a part of the player is on the given coordinates
* 
* @return The found char
* @return ICON_STREET if nothing is found
*/
char check_player_coordinates(int y, int x) {

	if (player_coordinates.y == y && (player_coordinates.x == x || player_coordinates.x + PLYR_LEN - 1 == x)) return ICON_TIRE;

	for (int i = 1; i < PLYR_LEN - 1; i++) {

		if (player_coordinates.y == y && player_coordinates.x + i == x) return ICON_CAR;
	}

	return ICON_STREET;
}


void reset_game() {

	char temp;

	//Reset player_coordinates
	player_coordinates.y = TRUE_STRT_ROW;
	player_coordinates.x = PLYR_STRT_COL;

	//Reset game_field
	for (int i = 0; i < TRUE_ROWS; i++) {

		for (int j = 0; j < LENGTH; j++) {

			if (j == 0 || j == LENGTH - 1 || i % 2 == 0) game_field[i][j] = '0';
			else if ((temp = check_player_coordinates(i, j)) != ICON_STREET) game_field[i][j] = temp;
			else game_field[i][j] = ICON_STREET;
		}
	}
}


/*
* Draws the game field each GAME_TICK
* Also draws borders around the game field
*/ 
void draw_game_field() {

	system("cls");

	for (int i = 0; i < TRUE_ROWS; i++) {

		for (int j = 0; j < LENGTH; j++) {

			if (j == 0 || j == LENGTH - 1) printf("|");
			else if (i % 2 == 0) printf("-");
			else printf("%c", game_field[i][j]);
		}

		printf("\n");
	}

	printf("\nScore: %d", score);
}


/*
* Moves player according to the given input and returns -1 if the player collides with an obstacle
* 
* @param input Vector 2 
* @return 0 if successful
* @return -1 on collision
*/
int move_player(Vector2 input) {

	int new_y = player_coordinates.y + (input.y * 2);
	int new_x = player_coordinates.x + input.x;
	int return_value = 0;

	//Prevents player moving outside the borders
	if (new_y > 0 && new_y < TRUE_ROWS - 1 && new_x > 0 && new_x < LENGTH - 1 - PLYR_LEN) {

		//Removes "old" car
		game_field[player_coordinates.y][player_coordinates.x] = ICON_STREET;
		game_field[player_coordinates.y][player_coordinates.x + PLYR_LEN - 1] = ICON_STREET;

		for (int i = 1; i < PLYR_LEN - 1; i++) {

			game_field[player_coordinates.y][player_coordinates.x + i] = ICON_STREET;
		}


		player_coordinates.y = new_y;
		player_coordinates.x = new_x;


		/*
		* Prints a "new" car to the new coordinates
		* Utilizes "return_value" variable instead of just returning to always print the full car, even on collision
		*/
		if (game_field[new_y][new_x] == ICON_STREET) game_field[new_y][new_x] = ICON_TIRE;
		else return_value = -1;

		if (game_field[new_y][new_x + PLYR_LEN - 1] == ICON_STREET) game_field[new_y][new_x + PLYR_LEN - 1] = ICON_TIRE;
		else return_value = -1;

		for (int i = 1; i < PLYR_LEN - 1; i++) {

			if (game_field[new_y][new_x + i] == ICON_STREET) game_field[new_y][new_x + i] = ICON_CAR;
			else return_value = -1;
		}
	}

	return return_value;
}


/*
* Moves the obstacles one step to the left and returns -1 if the obstacle collides with the player
* 
* @return 0 if successful
* @return -1 on collision with the player
*/
int move_game_field() {

	int return_value = 0;

	for (int i = 1; i < TRUE_ROWS - 1; i += 2) {

		//Increase score if an obstacle has reached the left end, meaning the player has successfully dodged it
		if (game_field[i][1] == ICON_OBSTCL) score++;

		/*
		* Move each obstacle individually one step to the left
		* Utilizes a "return_value" variable instead of just returning to always move all obstacles before returning
		*/
		for (int j = 1; j < LENGTH - 2; j++) {

			if (check_player_coordinates(i, j) == ICON_STREET && check_player_coordinates(i, j + 1) == ICON_STREET) game_field[i][j] = game_field[i][j + 1];
			if (check_player_coordinates(i, j) == ICON_STREET && check_player_coordinates(i, j + 1) != ICON_STREET) game_field[i][j] = ICON_STREET;
			else if (check_player_coordinates(i, j) != ICON_STREET && game_field[i][j + 1] == ICON_OBSTCL) {

				game_field[i][j] = game_field[i][j + 1];
				return_value = -1;
			}
		}

		game_field[i][LENGTH - 2] = ICON_STREET;
	}

	return return_value;
}


//Spawns an obstacle in a random row
void spawn_obstacles() {

	int row = random(1, ROWS) * 2 - 1;

	game_field[row][LENGTH - 2] = ICON_OBSTCL;
}


/*
* Checks if the player has pressed any of the defined keys
* 
* @return The input as Vector2
* @return (0,0) If no input was detected
* @return (1,1) If player presses the BTN_QUIT key
*/
Vector2 get_input() {

	Vector2 input = { 0, 0 };

	if (_kbhit()) {
		switch (getch()) {
		case BTN_UP:
			input.y = -1;
			break;
		case BTN_DOWN:
			input.y = 1;
			break;
		case BTN_FORWARD:
			input.x = 1;
			break;
		case BTN_BACKWARD:
			input.x = -1;
			break;
		case BTN_QUIT:
			input.x = 1;
			input.y = 1;
			game_over = 1;
			break;
		}
	}

	return input;
}


/*
* Hides text cursor, prevents flickering (mostly)
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

	long long int tick_count = 0;
	Vector2 input;
	long long int current_obstacle_speed = OBSTCLE_SPD_TICK;

	
	hide_cursor();
	reset_game();
	draw_game_field();

	//Main game loop
	while (!game_over) {
		
		//Get input
		input = get_input();

		//Quit if player has pressed BTN_QUIT
		if (input.x == 1 && input.y == 1) continue;

		//Move player
		if (move_player(input) == -1) {

			game_over = 1;
			continue;
		}

		//Move obstacles
		if (tick_count % current_obstacle_speed == 0 && move_game_field() == -1) {

			game_over = 1;
			continue;
		}

		//Spawn new obstacles
		if (tick_count % (current_obstacle_speed * OBSTCLE_SPAWN_TICK) == 0) spawn_obstacles();

		//Increase speed each time the score gets higher than a multiple of OBSTCLE_SPD_INCR_VAL
		if (current_obstacle_speed > 1 && score >= OBSTLCE_SPD_INCR_VAL) current_obstacle_speed = OBSTCLE_SPD_TICK - (score / OBSTLCE_SPD_INCR_VAL);

		//Draw the game field
		draw_game_field();

		//Increase tick count and wait GAME_TICK ms
		tick_count++;
		Sleep(GAME_TICK);
	}

	draw_game_field();

	printf("\nGame Over!");

	return 0;
}