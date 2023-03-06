/*
Basic implementation of a doubly-linked list.
*/

// ------- MEMORY LEAK DETECTION --------------
#ifdef _DEBUG
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
#endif
// --------------------------------------------

// Includes
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//Constants
#define MAX 5

// Typedefs
typedef struct q {						// definition of a self-referential structure used to create the linked list
	int number;                         // each q struct contains int data
	struct q* next;                     // pointer to the next item in the list
	struct q* prev;                     // pointer to the previous item in the list
} REGTYPE;  							// end struct q (alias REGTYPE)

// Function declarations 
REGTYPE* create_random_list(void);
REGTYPE* add_first(REGTYPE* temp, int data);
void print_element(REGTYPE* newPtr);

 /* Main program */
int main(int argc, char* argv[])
{
	// ------- MEMORY LEAK DETECTION --------------------------------
	#ifdef _DEBUG
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	#endif
	// --------------------------------------------------------------

	int nr = 0;                         // counter for the index of each element in the list
	REGTYPE* act_post;                  // declare an instance of the *struct to act as the navigation pointer
	REGTYPE* head = NULL;               // declare an instance of the *struct to act as the head of the list (initially no nodes exist)
	srand(time(0));                     // Random seed
	head = create_random_list();        // get the head of a random linked list 
	act_post = head;                    // set the navigation pointer to the head of the list

	while (act_post != NULL) {          // loop until the last item in the list is reached (i.e. its 'next' pointer points to nothing)
		printf("Post nr %d : %d", nr++, act_post->number);
		print_element(act_post);        // prints the element's next- and previous-pointer values
		act_post = act_post->next;      // increment the pointer to point at the next item in the list
	}

	// free the allocated memory
	while ((act_post = head) != NULL) { // set the navigation pointer to point to head
		head = act_post->next;          // increment the head pointer to point to the next item in the list
		free(act_post);                 // free the memory pointed to by the navigation pointer
	}

	return 0;
}

/*
* Creates a linked list with a specified number of records of type REGTYPE.
* The value of the 'data' variable is given a random number from 0-100.
*/
REGTYPE* create_random_list(void) {
	int nr, i = 0;
	REGTYPE* top, * last, * item;
	top = last = NULL;

	for (; i < MAX; i++) {							// keep adding elements to the list until MAX elements have been inserted
		item = malloc(sizeof(REGTYPE));				// create and allocate memory for a new node
		nr = rand() % 100 + 1;						// generate data: a new random number
		item->number = nr;							// place the data in the new node

		if (top == NULL) {							// the list is currently empty
			top = last = item;						// only 1 element in the list, all pointers point to the new node         
			last->next = NULL;						// 'last' is the last item in the list
			top->prev = NULL;						// 'top' is the first item in the list
		}

		else if (last->next == NULL) {				// the new node will be added to the end of the list 
			last->next = item;						// insert the new node 'after' the last node
			item->prev = last;						// the 'prev' node of 'item' points to 'last'
			item->next = NULL;						// the new node is the last node in the list
			last = item;							// last now points to the last item in the list

		}
	}
	return(top);                                 	// return the pointer pointing to the start of the list
}
//==========================================================
/*
* Adds a new record to the first position of the list and assigns the value of 'data' to the struct field 'number'.
* @returns a pointer to the new first entry in the list.
*/
REGTYPE* add_first(REGTYPE* temp, int data) {
	REGTYPE* newPtr;                        		// pointer to the new node to insert
	REGTYPE* currentPtr;                    		// pointer to the current node in the list
	REGTYPE* nextPtr = NULL;                		// pointer to the previous node in the list

	newPtr = malloc(sizeof(REGTYPE));				// allocate memory for the new node

	if (newPtr != NULL) {							// if there is space available
		newPtr->number = data;              		// place the data in the new node
		newPtr->next = NULL;                		// the new node does not (yet) point to a previous node
		newPtr->prev = NULL;						// the new node will be the first item in the list

		currentPtr = temp;                  		// place the navigation pointer at the place the temp pointer is pointing to
		// (assumption is that the temp pointer could be any item in the list)

		while (currentPtr != NULL) {        		// loop until we get to the first element (i.e. its 'previous' pointer is NULL)
			nextPtr = currentPtr;					// set the next pointer to the position of the current pointer
			currentPtr = currentPtr->prev;			// navigate to the previous node until we reach the first node
		}

		if (nextPtr == NULL) {              		// the list is currently empty so insert at the beginning of the list
			newPtr->next = temp;					// the new node link points to the former first node
			temp = newPtr;							// make the provided pointer 'temp' point to the new node

			#ifdef _DEBUG
				print_element(newPtr);
			#endif
		}

		else if (currentPtr == NULL) {      		// reached the beginning of the list (first node has no previous links)
			newPtr->next = nextPtr;					// the new element's 'next' pointer points to the (now) second element
			nextPtr->prev = newPtr;					// the (now) second element's 'previous' pointer points to the new element
			newPtr->prev = NULL;					// the new pointer is now the first element in the list (has no prev link)        

			#ifdef _DEBUG
				print_element(newPtr);
			#endif
		}
	}
	else {
		printf("\nElement [%d] not inserted: no memory available\n", data);
	}


	return newPtr;  								// return the new node that points to the start/head of the linked list
}

/*
* Prints information about a given node: its data, its 'next' pointer's data and its 'previous' pointer's data.
* If the node does not have a next or previous pointer, a string "null" is printed instead of a value.
*/
void print_element(REGTYPE* newPtr) {

	#ifdef _DEBUG	
		printf(" <Inserted new node with data [%d]> ", newPtr->number);
	#else
		fputs("      | ", stdout);
	#endif

	if (newPtr->prev == NULL)						// check if the node's previous pointer is NULL (e.g. the node is the first element)
		fputs("Prev: [null], ", stdout);
	else
		printf("Prev: [%d], ", newPtr->prev->number);

	if (newPtr->next == NULL)						// check if the node's next pointer is NULL (e.g. the node is the last element)
		fputs("Next: [null]\n", stdout);
	else
		printf("Next: [%d]\n", newPtr->next->number);

}

