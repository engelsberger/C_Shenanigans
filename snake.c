#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <windows.h>

/*
* *** Game settings ***
* 
* SNK_DEF_LEN	.. The length the snake starts with
*/
#define WIDTH 20
#define HEIGHT 15
#define SNK_DEF_LEN 5
#define ICON_HEAD '0'
#define ICON_BODY '*'
#define ICON_FRUIT '+'

/*
* *** Input settings ***
*/
#define KEY_UP 'w'
#define KEY_DOWN 's'
#define KEY_LEFT 'a'
#define KEY_RIGHT 'd'
#define KEY_ABORT 'x'


typedef struct {
	int x;
	int y;
} Vector2;

Vector2 snake_coordinates[(WIDTH - 2) * (HEIGHT - 2)];
Vector2 fruit_coordinates = { 0, 0 };
int snake_length = SNK_DEF_LEN;
int game_over = 0;


/*
* Hides text cursor in the console for less flickering
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
* Checks if there is a part of the snake in the given coordinates
* 
* @param x X-Coordinate
* @param y Y-Coordinate
* @return 0 if no match is found
* @return 1 if ICON_BODY is found
* @return 2 if ICON_HEAD is found
* @return 3 if ICON_FRUIT is found
*/
int check_coordinates_for_icons(int x, int y) {

	if (fruit_coordinates.x == x && fruit_coordinates.y == y) return 3;

	if (snake_coordinates[0].x == x && snake_coordinates[0].y == y) return 2;

	for (int i = 1; i < snake_length; i++) {

		if (snake_coordinates[i].x == x && snake_coordinates[i].y == y) return 1;
	}

	return 0;
}


//Draws the game field
void draw_game_field() {

	system("cls");

	for (int i = 0; i < HEIGHT; i++) {

		for (int j = 0; j < WIDTH; j++) {

			if (j == 0 || j == WIDTH - 1) printf("|");
			else if (i == 0 || i == HEIGHT - 1) printf("-");
			else if (check_coordinates_for_icons(j, i) == 3) printf("%c", ICON_FRUIT);
			else if (check_coordinates_for_icons(j, i) == 2) printf("%c", ICON_HEAD);
			else if (check_coordinates_for_icons(j, i) == 1) printf("%c", ICON_BODY);
			else printf(" ");
		}

		printf("\n");
	}

	printf("\nScore: %d\n", snake_length - SNK_DEF_LEN);

	if (game_over) printf("GAME OVER!\n");
}


//Completely reset the snake_coordinates array and the snake_length integer to default values
void reset_snake_coordinates() {

	snake_length = SNK_DEF_LEN;

	for (int i = 0; i < (WIDTH - 2) * (HEIGHT - 2); i++) {

		if (i < snake_length) {

			snake_coordinates[i].y = HEIGHT / 2;
			snake_coordinates[i].x = WIDTH / 2 - i;
		}
		else {

			snake_coordinates[i].y = 0;
			snake_coordinates[i].x = 0;
		}
	}
}


/*
* Takes the player input and checks if snake collides with itself or a fruit
* 
* @param input Input
* @return 0 For no collision
* @return 1 For collision with fruit -> grow
* @return 2 For collision with itself -> gameover
*/
int check_for_collision(Vector2 input) {

	if (check_coordinates_for_icons(snake_coordinates[0].x + input.x, snake_coordinates[0].y + input.y) == 1) return 2;
	else if (check_coordinates_for_icons(snake_coordinates[0].x + input.x, snake_coordinates[0].y + input.y) == 3) return 1;

	return 0;
}


/*
* Updates the snake_coordinates according to the given input ("moves" the snake)
* 
* @param input The input vector according to which the snake moves
* @param grow If true (1), snake grows by one
*/
void update_snake_coordinates(Vector2 input, int grow) {

	snake_length += grow;

	for (int i = snake_length - 1; i > 0; i--) {

		snake_coordinates[i].x = snake_coordinates[i - 1].x;
		snake_coordinates[i].y = snake_coordinates[i - 1].y;
	}

	snake_coordinates[0].x += input.x;
	snake_coordinates[0].y += input.y;

	if (snake_coordinates[0].y < 1) snake_coordinates[0].y = HEIGHT - 2;
	else if (snake_coordinates[0].y > HEIGHT - 2) snake_coordinates[0].y = 1;
	else if (snake_coordinates[0].x < 1) snake_coordinates[0].x = WIDTH - 2;
	else if (snake_coordinates[0].x > WIDTH - 2) snake_coordinates[0].x = 1;
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


//Set the fruit_coordinate Vector to a new empty field
void set_random_fruit() {

	fruit_coordinates.x = random(1, WIDTH - 2);
	fruit_coordinates.y = random(1, HEIGHT - 2);

	for (int i = 0; i < 100000 && check_coordinates_for_icons(fruit_coordinates.x, fruit_coordinates.y) != 0; i++) {

		fruit_coordinates.x = random(1, WIDTH - 2);
		fruit_coordinates.y = random(1, HEIGHT - 2);
	}
}


/*
* Takes the player input (WASD) and writes it into a Vector2
* 
* @return The given input as Vector2
* @return (0,0) if no input is detected
* @return (1,1) if player presses the quit button
*/
Vector2 get_input() {

	Vector2 input = { 0, 0 };

	if (_kbhit()) {
		switch (getch()) {
		case KEY_UP:
			input.y = -1;
			break;
		case KEY_DOWN:
			input.y = 1;
			break;
		case KEY_RIGHT:
			input.x = 1;
			break;
		case KEY_LEFT:
			input.x = -1;
			break;
		case KEY_ABORT:
			input.x = 1;
			input.y = 1;
			game_over = 1;
			break;
		}
	}

	return input;
}


/*
* main function
* 
* @return 0 if successful
*/
int main(int argc, char* argv[]) {
	
	Vector2 prev_input = { 1, 0 };


	//Reset game
	hide_cursor();
	set_random_fruit();
	reset_snake_coordinates();

	//Start a Countdown
	for (int i = 3; i > 0; i--) {

		draw_game_field();
		printf("\nGame starting in %d...\n\n", i);

		Sleep(800);
	}
	
	//main loop
	while (!game_over) {

		Vector2 input = get_input();
		
		//Prevent attempts to move in the opposite direction (when snake is moving up, it cannot move down immediately)
		if (input.x == 1 && input.y == 1) continue;
		else if (input.x == 0 && input.y == 0) input = prev_input;
		else if (input.x == -prev_input.x || input.y == -prev_input.y) input = prev_input;

		prev_input = input;

		int collision = check_for_collision(input);

		if (collision == 2) {
			game_over = 1;
			continue;
		}
		else if (collision == 1) {
			set_random_fruit();
		}

		update_snake_coordinates(input, collision);
		draw_game_field();

		Sleep(80);
	}

	draw_game_field();

	return 0;
}