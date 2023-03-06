/*
A program that reads a maximum amount of characters from stdin. The string is copied 
using a custom function and using the strcpy() function.

The input can be read from:
 - the keyboard during runtime (no args passed)
 - arguments passed on the command line e.g. main TextToPass 
 - from redirected file input e.g. main < myfile.txt
*/

//Include section
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Define section
#define MAX_SIZE 20

// Function declaration
void copy_string(char input[], char* copy, size_t buf_size);

/* Main program */
int main(int argc, char** argv) {
	char input[MAX_SIZE] = { 0 };				// create a buffer to hold the input from console or a file (static memory)
	char string_copy[MAX_SIZE];					// create a string to copy the input into using strcpy() function
	char function_copy[MAX_SIZE];				// create a string to copy the input into using a custom function

	// Input is from argv argument
	if (argc > 1) {								// if there is a command line parameter, this is the input

		puts("argc > 1\n");
		size_t i;
		for (i = 0; i < MAX_SIZE; i++) {		// copy up to MAX_SIZE characters
			input[i] = argv[1][i];				// use the first (non-filename) parameter as the input
		}

		if (input[MAX_SIZE - 1] != '\0') {		// ensure the string has a null-terminating char
			input[MAX_SIZE - 1] = '\0';
		}
	}
	// Input is from redirected file input or entered via the keyboard during runtime
	else if (fgets(input, MAX_SIZE, stdin) == NULL) {	
		puts("\nError! Could not read input.");
		return 2;
	}
	// Copy the string
	if (input != NULL) {								// execute copies if the read was successful
		strcpy_s(string_copy, MAX_SIZE, input);			// copy the content of the input using strcpy_s (avoid buf overflow)
		copy_string(input, function_copy, MAX_SIZE);	// copy the content of the input using a custom function

		// print out the strings to console
		printf("strcpy   : %s\n", string_copy);
		printf("func copy: %s\n", function_copy);
	}
}

/* Custom function to copy a string */
void copy_string(char input[], char* copy, size_t buf_size) {	
	size_t i;							// counter used in the foor loop
	for (i = 0; i < buf_size; i++) {	// iterate through each character in the string
		copy[i] = input[i];				// copy each character from the given string into the copy
	}
	if (copy[buf_size -1] != '\0') {	// ensure the string has a null-terminating char
		copy[buf_size -1] = '\0';
	}
}