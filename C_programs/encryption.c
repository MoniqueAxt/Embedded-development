/** A simple encryption program that shifts each character of a text by
* a specified amount (Caesar Shift Cipher).
* 
* Demonstration of basic string/char handling and manipulation.
*/

// Include section
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

// Define section
#define SIZE 80	

/* Main program */
void main(int argc, char** argv)
{
	int shift = 13;					// default value used if the user doesn't specify an argument
	int c = -1;						// variable to hold character entered by user
	int encryptedChar = -1;			// variable to hold the encrypted char
	char encryptedText[SIZE];		// created a char array to store the entire encrypted text
	int i = 0;						// initialise counter used in the while loop

	// if provided, the argument given by the user is used as the shift
	if (argc > 1)
	{
		shift = atoi(argv[1]);		// cast the 2nd argument (the shift number) to an int
	}

	puts("Enter text to encrypt:");	// prompt the user for input
	c = getchar();					// get the next character from input

	// loop the program until an EOF char is reached
	while (c!= EOF)
	{
		// loop reading chars until the size limit is reached or the user presses enter
		while (i < SIZE - 1 && c != '\n')
		{
			// a-z (97-122)
			if (islower(c))						// if the character is lowercase, use lowercase conversion logic
			{
				encryptedChar = c - 'a';		// subtract 97 to so the index of letters is 0-25 (instead of 97-122)
				encryptedChar += shift;			// shift the char the specified amount of indexes
				encryptedChar %= 26;			// circular shift any char indexes that go above 25
				encryptedChar += 'a';			// add back 97 ('a') to get the correct ASCII range 97-122
			}
			// A-Z (65-90)
			else if (isupper(c))				// if the character is uppercase, use uppercase conversion logic
			{
				encryptedChar = c - 'A';		// subtract 65 to so the index of letters is 0-25 (instead of 65-90)
				encryptedChar += shift;			// shift the char the specified amount of indexes
				encryptedChar %= 26;			// circular shift any char indexes that go above 25
				encryptedChar += 'A';			// add back 65 ('A') to get the correct ASCII range 65-90
			}

			encryptedText[i++] = encryptedChar;	// add the encrypted character to the array that holds the entire encrypted string
			c = getchar();						// update the char variable to the next character in input
		}
		// append a null char to the array after reading terminates so that it can be printed as a string
		encryptedText[i] = '\0';
		printf("\tEncrypted text:\n\t%s\n", encryptedText);		
		
		// reset the counter so the encrypted string's index begins at 0 again for a new text
		i = 0;
		
		puts("\nEnter text to encrypt:");		// prompt the user for input
		c = getchar();							// get the next character from input
	}

	puts("\nGoodbye!");						

}

