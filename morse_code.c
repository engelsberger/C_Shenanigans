#include <stdio.h>
#include <Windows.h>


#define MAX_ITERATIONS 1000000

//DIT .. Delay in milliseconds
#define DIT 150
#define DAH (DIT * 3)
#define DELAY_SYMBOL (DIT * 1)
#define DELAY_LETTER (DIT * 3)
#define DELAY_WORDS (DIT * 7)


typedef struct {
	char letter;
	char* code;
} Morse_Code;


Morse_Code morse_code[] = {
	{'A', ".-"}, {'B', "-..."}, {'C', "-.-."}, {'D', "-.."}, {'E', "."}, {'F', "..-."}, {'G', "--."},
	{'H', "...."}, {'I', ".."}, {'J', ".---"}, {'K', "-.-"}, {'L', ".-.."}, {'M', "--"}, {'N', "-."},
	{'O', "---"}, {'P', ".--."}, {'Q', "--.-"}, {'R', ".-."}, {'S', "..."}, {'T', "-"}, {'U', "..-"},
	{'V', "...-"}, {'W', ".--"}, {'X', "-..-"}, {'Y', "-.--"}, {'Z', "--.."}, {'0', "-----"}, {'1', ".----"},
	{'2', "..---"}, {'3', "...--"}, {'4', "....-"}, {'5', "....."}, {'6', "-...."}, {'7', "--..."}, {'8', "---.."},
	{'9', "----."}
};
int morse_code_count = sizeof(morse_code) / 8;


/*
* Converts a given char to uppercase
* 
* @param input The given input
* @return The given input as uppercase char, if it is a lowercase char
* @return The given input, if it is no lowercase char
*/
char to_uppercase(char input) {

	if (input >= 'a' && input <= 'z') return input + 'A' - 'a';

	return input;
}


char* convert_letter_to_morse(char letter) {

	for (int i = 0; i < morse_code_count; i++) {

		if ((morse_code + i)->letter == letter) return (morse_code + i)->code;
	}

	return "";
}


char* convert_string_to_morse(char* input, int input_length) {

	char* output = (char*)malloc(input_length * 5 * sizeof(char));
	char temp;
	char* morse;
	int length = 0;
	int morse_valid = 0;

	for (int i = 0; i < MAX_ITERATIONS && *(input + i) != '\0'; i++) {

		temp = to_uppercase(*(input + i));

		if (temp == ' ') {

			*(output + length) = '|';
			length++;
			continue;
		}

		morse = convert_letter_to_morse(temp);

		for (int j = 0; j < MAX_ITERATIONS && *(morse + j) != '\0'; j++) {

			*(output + length) = *(morse + j);
			length++;
			morse_valid = 1;
		}

		if (morse_valid && !(*(input + i + 1) == '\n' || *(input + i + 1) == ' ')) {
			*(output + length) = ' ';
			length++;
			morse_valid = 0;
		}
	}

	*(output + length) = '\0';

	return output;
}


int main(int argc, char* argv[]) {

	char* input;
	char* output;
	char temp;
	int input_length = 0;
	int sleep_dur = DIT;


	if (argc != 2) {
		printf("Wrong number of arguments!\n");
		return 1;
	}

	input = argv[1];
	for (input_length = 0; input_length < MAX_ITERATIONS && *(input + input_length) != '\0'; input_length++);

	output = convert_string_to_morse(input, input_length);

	for (int i = 0; i < MAX_ITERATIONS && *(output + i) != '\0'; i++) {

		temp = *(output + i);

		printf("%c", temp);

		switch (temp) {
		case '.':
			sleep_dur = DIT;
			break;
		case '-':
			sleep_dur = DAH;
			break;
		case ' ':
			sleep_dur = DELAY_LETTER;
			break;
		case '|':
			sleep_dur = DELAY_WORDS;
			break;
		}

		Sleep(sleep_dur + DELAY_SYMBOL);
	}

	printf("\n");

	return 0;
}