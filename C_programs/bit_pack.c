/*
Demonstration of bit packing: packing bits into a byte.
A way to consolidate multiple data values into a single variable (data compression).
*/

// Include section
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define CHAR_BIT 8;

// Typedefs
typedef struct                      // Struct that specifies the number of bits in which unsigned int fields are stored
{
    unsigned int engine_on  : 1;    // holds 1 bit  - the engine is on or off. bit no 7 (MSB, value 128 decimal).
    unsigned int gear_pos   : 3;    // holds 3 bits - the position of the gear. 3 bits = 2^3 = 7 positions
    unsigned int key_pos    : 2;    // holds 2 bits - the position of the key. 0=stop, 1=on, 2=engine starter on
    unsigned int brake1     : 1;    // holds 1 bit  - position of the front brakes
    unsigned int brake2     : 1;    // holds 1 bit  - position of the rear brakes. bit no 0 (LSB).
} CAR;

typedef unsigned char byte;         // define a new type byte

// Function definitions
void printbits(unsigned char a, char* msg);
int valid_input(const char* input[], const int size);

/*
* Main program
* Takes 5 arguments - more is an error
*/
int main(int argc, char* argv[])
{    
    byte packed_byte = 0b00000000;   // initialise an empty byte
    CAR car;                         // create an instance of a CAR structure

    // skip over the program name
    //--argc;           // exclude program name being counted in the number of arguments
    //++argv;           // exclude program name from the arguments provided

    // validate that exactly 5 argument were provided (excluding the file name)
    if (argc != 6) {
        puts("Error: too few or too many arguments. This program accepts exactly 5 arguments.\n");
        return(2);
    }

    // validate that all arguments are valid 1-digit numbers
    if (!valid_input(argv, argc)) {
        return(2);
    }
        
    // print out the arguments provided
    int i = 0;
    fputs("Arguments: \t", stdout);
    for (i = 1; i < argc; i++) {
        printf(argv[i]);
    }
    puts("\n");

    car.engine_on = atoi(argv[1]);                  // set the status of the car's engine
    car.gear_pos = atoi(argv[2]);                   // set the status of the car's gear position
    car.key_pos = atoi(argv[3]);                    // set the status of the car's key position
    car.brake1 = atoi(argv[4]);                     // set the status of the car's brake 1
    car.brake2 = atoi(argv[5]);                     // set the status of the car's brak 2
 
    // left-shift the bits to their respective position in the byte by using the OR operator on the byte
    printbits(packed_byte, "Original: \t");
    packed_byte |= car.engine_on << 7;              // shift the engine_on bit to the MSB (bit no 7) of the byte
    printbits(packed_byte, "shift to MSB: \t");
    packed_byte |= car.gear_pos << 4;               // shift gear_pos bits by 4 places in the byte
    printbits(packed_byte, "shift 4 bits: \t");
    packed_byte |= car.key_pos << 2;                // shift key_pos bits by 2 places in the byte
    printbits(packed_byte, "shift 2 bits: \t");
    packed_byte |= car.brake1 << 1;                 // shift the brake1 bit by 1 place in the byte
    printbits(packed_byte, "shift 1 bit: \t");
    packed_byte |= car.brake2;                      // place the brake2 bit at the LSB (bit no 0) of the byte
    printbits(packed_byte, "set LSB: \t");

    puts("\nName\t\tValue");
    puts("-----------------------");
    printf("engine_on\t%d   Bit no 7\n", car.engine_on);
    printf("gear_pos \t%d\n", car.gear_pos);
    printf("key_pos  \t%d\n", car.key_pos);
    printf("brake1   \t%d\n", car.brake1);
    printf("brake2   \t%d   Bit no 0\n", car.brake2);

    puts("\n");
    printf("Hex : %02X\n", packed_byte);            // print the packed byte in hexadecimal form
    printbits(packed_byte, "Bin : ");               // print the packed byte in binary form

    return 0;
}
/*
* Print a byte in binary form to the output
*/
void printbits(unsigned char a, char* msg) {
    printf(msg);
    int i; 
   // for (i = 7; i >= 0; i--) putchar('0' + ((v >> i) & 1));
    for (i = 0; i < 8; i++) {
        printf("%d", !!((a << i) & 0x80));
    }
    puts("");
}
/*
* Validate that the input is a numeric digit and within its allowable range
*/
int valid_input(const char* input[], const int size) {
    for (int i = 1; i < size; i++)
    {
        int num;

        if (!sscanf_s(input[i], "%d", &num)) {                  // check that the input arguments are numeric digits
            puts("Invalid input: Non-numeric input.");
            return 0;
        }
        
        int single_bit_field = (i == 1 || i == 4 || i == 5);    // check if we're evaluating the first, forth, or fith argument

        if (single_bit_field && (num < 0 || num > 1)) {         // engine_on, brake1 and brake2 fields accept only 0 or 1
            puts("Invalid input on first, fourth or fifth argument. Value can only be 1 or 0.");
            return 0;
        }
        
        if (i == 2 && (num < 0 || num > 7)) {                   // gear_pos field accepts values from 0 to 7
            puts("Invalid input on second argument. Value can only be a number from 1-7.");
            return 0;
        }
        
        if (i == 3 && (num < 0 || num > 2)) {                   // key_pos field accepts only 0, 1 or 2
            puts("Invalid input on third argument. Value can only be 0, 1 or 2");
            return 0;
        }
 
    }
    return 1;
}
