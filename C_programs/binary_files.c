/*
Operations on binary files: create, append, read records.
A console-based menu-driven program that imitates managing a database of people.
The data is stored as a binary file.
*/

// Include section
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

// Define section
#define BUFFER_SIZE 50

// Constants
char FILENAME[] = "persons.dat";

// Typedefs
typedef struct {
	char firstname[20];
	char famname[20];
	char pers_number[13];	// yyyymmddnnnc
} PERSON;

// Function declarations
PERSON add_record(void);				// Add a new Person record.
int create_file(PERSON* inrecord);			// Creates a file and writes the first record
int append_to_file(PERSON* inrecord);			// Appends a new person to the file
void search_by_name(char* name, char c);		// Prints out the person if in the database
void print_file(void);					// Prints out all persons in the file
int get_valid_input();					// Gets and validates input


/*
* A program that manages a database of persons stored as a binary file.
*/
int main(void) {
	puts("----- Database Management -----\n\n");

	int menu_input;

	do {		
		puts("1. Create a new file and delete the old file");
		puts("2. Add a new person to the file");
		puts("3. Search for a person in the file");
		puts("4. Print out all in the file");
		puts("5. Exit the program");
		
		menu_input = get_valid_input();								// get and validate input from the user
		
		switch (menu_input) {
		// Create a new file (deletes the old file)
		case 1:	
		{		
			PERSON dummy_record = { "firstname", "famname", "00000000000" };		// create a dummy record to insert into the created file
			if (create_file(&dummy_record)) {						// check if the dummy record was successfully written
				puts("Successfully created the file and inserted a dummy record.\n");
			}
			else {
				puts("Error: could not write the dummy record to file.\n");
			}
		}
		break;
		// Add a new person to the file
		case 2:
		{			
			puts("\n--- Add a record to the file ---");
			PERSON person_to_add = add_record();						// get the record that will be written to the file
			// check that all variables in the struct have valid data
			bool person_added = person_to_add.firstname[0] != 0 && person_to_add.famname[0] != 0 && person_to_add.pers_number[0] != 0;

			// check that the person was created with valid input and the record was successfully appened to the file
			if (!person_added || !append_to_file(&person_to_add)) {
				puts("Couldn't add person!\n\n");
			}
			else {
				printf("\nAdded %s %s to the database!\n\n", person_to_add.firstname, person_to_add.famname);
			}
		}
		break;
		// Search for a person in the file by first or last name
		case 3:	
		{
			char name_to_search[BUFFER_SIZE];
			char c[3];	// char array of size 3 to hold input char, newline and terminating char
			// let the user choose to search for first name or last name
			fputs("Do you want to search for [f]irstname or [l]astname?: ", stdout);
			fgets(c, sizeof(c), stdin);

			// check that the user entered a valid choice
			if (c[0] != 'f' && c[0] != 'l') {
				puts("Invalid choice. Enter 'f' for firstname or 'l' for lastname.\n");
				break;							// break out of the switch statement if invalid input was entered
			}
			
			fputs("Enter name to search: ", stdout);
			if (fgets(name_to_search, BUFFER_SIZE, stdin) == NULL) {	// read the name to search for from stdin
				puts("Error reading input!");
				exit(1);						// exit the program with failure
			}
			puts("\n--- Printing all matches ---");
			name_to_search[strcspn(name_to_search, "\n")] = 0;		// exclude the trailing newline char by using strcspn to set the last char to a null-terminating char
			search_by_name(name_to_search, c[0]);
			puts("\n\n--------- end  ----------n");
		}
		break;
		// Print out all the persons in the file
		case 4:	
		{
			puts("\n--- Printing all data from file ---");
			print_file();
			puts("\n\n----------- end of file -----------\n");
		}
		default: break;								// Invalid input entered or the user chose to exit the program
		}

	} while (menu_input != 5);							// loop as long as the user doesn't exit the program


	return(0);
}
/*
* Creates a new file (overwrites if existing), writes a first dummy record to the file and then closes it.
* @return zero if the dummy record was successfully written to the file, else a non-zero value on failure
*/
int create_file(PERSON* inrecord) {
	puts("\nwrite new file");

	FILE* fileptr;									// pointer used to point to the file once opened

	if ((fileptr = fopen(FILENAME, "wb")) == NULL) {				// attempt to open a file for writing binary ("wb")
		puts("\nError: Cannot create file");
		exit(1);								// exit the program with failure
	}
	// args: pointer to start of data block, size of block, nr of block, file pointer)
	int nr_written = fwrite(inrecord, sizeof(PERSON), 1, fileptr);	// write the data starting at the pointer to the data block using the filepointer
	fclose(fileptr);								// closes the file stream and flushes all buffers

	if (nr_written != 1) {								// fwrite returns how many objects it wrote, if zero then the data was not written
		return false;
	}
	
	return true;									// data was successfully written to file
}
/*
* Gets data from the input in order to create a new person.
* @return a PERSON struct with data if successful, or
* a ZERO-initialised PERSON struct if the input used to initialise the struct is invalid
*/
PERSON add_record() {
	char input_buffer[BUFFER_SIZE];							// buffer to store the first and last name entered in input
	PERSON person = { 0 };								// intialise struct to zero in order to check valid initialising input later
	int terminal_char_pos = 0;
	bool person_data_invalid = false;						// keep track of any invalid input the user enters

	fputs("Enter first and last name: ", stdout);					// read the input from the the stdin stream
	if (fgets(input_buffer, BUFFER_SIZE, stdin) == NULL) {				// read the names from input into the struct
		puts("Error reading input!");
		exit(1);								// exit the program with failure
	}
	// get the first and last names separated by a space from the buffer and place it into the struct 
	int arguments_filled = sscanf(input_buffer, "%s %s%n", &person.firstname, &person.famname, &terminal_char_pos);

	// validate that sscanf filled both arguments passed and that the expected input (str) was received
	if (arguments_filled != 2 || input_buffer[terminal_char_pos] != '\n') {
		puts("Error: invalid input\n");
		person_data_invalid = true;						// flag that data is invalid (e.g. if 2 names were not provided)
	}
	// the previous input was valid
	else {		
		fputs("Enter your person number: ", stdout);
		if (fgets(&person.pers_number, BUFFER_SIZE, stdin) == NULL) {		// read the person number from input into the struct
			puts("Error reading input!");
			exit(1);							// exit the program with failure
		}
		// validate the person number input
		for (int i = 0; i < strlen(person.pers_number); i++) {			// loop through the person number chars
			if (!isdigit(person.pers_number[0])) {				// check that each char is a numeric digit
				puts("Error, invalid person number.\n");
				person_data_invalid = true;				// flag that an invalid person number was inputted
				break;							// break out of the loop as soon as invalid data is found
			}
		}
	}
	// if any input is invalid, reset the person struct
	if (person_data_invalid) {				
		person.firstname[0] = '\0';						// add a null-terminating character to the start of each array		
		person.famname[0] = '\0';			
		person.pers_number[0] = '\0';		
	}
	
	return person;									// return the person to be added
}
/*
* Reads data from an existing binary file, outputs the contents, and closes the file.
*/
void print_file() {
	FILE* fileptr;									// pointer to the file to read
	PERSON initialised_person = { "firstname", "secondname", "00000000-0000" };	// initialise a default Person struct
	int counter = 0;								// create a counter to print before each record is printed

	if ((fileptr = fopen(FILENAME, "rb")) == NULL) {				// attempt to open and read the binary file ("rb")
		printf("\nError: Cannot open file");
		exit(1);								// exit the program with failure
	}
	// Read the contents of the entire file
	while (!feof(fileptr)) {							// read until the end of the file
		int nr_read = fread(&initialised_person, sizeof(PERSON), 1, fileptr);	// read from the file and store the number of objects read

		if (nr_read > 0) {
			printf("\n[%i] %s %s", counter, initialised_person.firstname, initialised_person.famname);		
			counter++;							// increment the index counter shown before each print out		
		}
	}
	fclose(fileptr);								// closes the file stream and flushes all buffers
}

/*
* Searches through the file and finds all records matching the provided first or last name.
* Prints all matching records.
*/
void search_by_name(char* name, char c) { 
	FILE* fileptr;									// pointer used to point to the file once opened
	PERSON record;									// struct to temporarily hold data read from file
	int counter = 0;								// create a counter to print before each record is printed

	if ((fileptr = fopen(FILENAME, "rb")) == NULL) {				// attempt to open a file for reading binary ("rb")
		puts("\nError: Cannot create file");
		exit(1);								// exit the program with failure
	}

	while (!feof(fileptr)) {							// read until the end of the file
		fread(&record, sizeof(record), 1, fileptr);				// read the record from the file and store it in the provided variable

		if (c == 'f') {								// the user chose to search for firstnames
			if (strcmp(record.firstname, name) == 0) {			// compare the given firstname with the record firstname
				printf("\n[%s] %s", record.firstname, record.famname);
				counter++;						// increment the counter shown before each print out	
			}
		}
		else if (c == 'l') {							// the user chose to search for lastnames
			if (strcmp(record.famname, name) == 0) {			// compare the given lastname with the record lastname
				printf("\n[%s], %s", record.famname, record.firstname);
				counter++;						// increment the counter shown before each print out	
			}
		}		
	}

	fclose(fileptr);								// closes the file stream and flushes all buffers
}

/*
* Appends one new Person to the end of the file
* (Gives an opportunity to put in one new person to a temp record and then add this record in the end of the file.)
* @return zero if the record was successfully appened to the file, else a non-zero value on failure
*/
int append_to_file(PERSON* inrecord) { 
	FILE* fileptr;									// pointer used to point to the file once opened
	
	if ((fileptr = fopen(FILENAME, "ab")) == NULL) {				// attempt tp open the file in "append binary" mode ("ab")
		printf("\nError: Cannot open file");
		exit(1);								// exit the program with failure
	}
	// args: (pointer to start of data block, size of block, nr of block, file pointer)
	int nr_written = fwrite(inrecord, sizeof(PERSON), 1, fileptr);			// write the data starting at the pointer to the data block using the filepointer

	if (nr_written != 1) {								// fwrite returns how many objects it wrote: if zero, the data was not written
		return false;
	}
	fclose(fileptr);								// closes the file stream and flushes all buffers
	return true;									// data was successfully written to file
}

/*
* Continuously reads and validates user input until valid input is read.
* The input from the user is used to control the execution of different menu options.
* Only one number in range 1-5 is considered valid input.
* 
* @returns validated integer input
*/
int get_valid_input() {
	char input_buffer[BUFFER_SIZE];
	int i = -1;
	int terminal_char_pos = 0;
	bool valid_input = true;

	do {
		// user entered invalid input
		if (!valid_input) {							
			fputs("\nInvalid input. Enter one number from 1-5: ", stdout);
			valid_input = true;						// reset the variable to allow the user to input again
		}
		// prompt the user for input
		else {
			fputs("\nSelect option [1-5]: ", stdout);
		}

		// get input from the user by reading from stdin stream into the array specified
		// (a null pointer is returned from fgets if an error occurs)
		if (fgets(input_buffer, BUFFER_SIZE, stdin) == NULL) {		
			puts("Error reading input");						
			valid_input = false;
		}
		
		// if the return from sscanf is not 1, then an extra or non-numeric character was entered into input.
		int arguments_filled = sscanf(input_buffer, "%d%n", &i, &terminal_char_pos);

		if (arguments_filled != 1 || input_buffer[terminal_char_pos] != '\n') {	// check that only 1 char was inputted
			valid_input = false;
		}
		// validate that the input is one number between 1-5
		if (i < 1 || i > 5) {
			valid_input = false;
		}
		// if all conditions are met, the input is valid
		else {
			valid_input = true;
		}
	
	} while (!valid_input); // continue looping until a valid input is given

	return i;		// return the validated input number
}
