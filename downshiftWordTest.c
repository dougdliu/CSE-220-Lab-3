/*Authhors: Aaron Chasse, Jake Borg, Doug Liu
 *CSE 220 TTH 1030-1145 : Lab 3
 *
 *Program description: This program will be testing the function downshift_word
*/


#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "print.h"
#include "scanner.h"

//function prototypes
static void downshift_word(char *dPtr);
//global src_ptr is initialized to NULL
static *src_ptr = NULL;

int main( )
{


	char string_test[5] = "TeSt";
	Token *token = malloc(sizeof(Token));

	//valid test
	downshift_word(string_test);
	//invalid test
	downshift_word(token);
    return 0;
}

static void downshift_word(char *dPtr)
{
    /*
     Make all of the characters in the incoming word lower case.
     */
	//continue to loop until end of string is reached
	while(*dPtr != '\0')
	{
		//dereference what dPtr is looking at, convert to lower, and replace old char with new
		*dPtr = tolower(*dPtr);
		dPtr++; //increment pointer to look at next character
	}
}
