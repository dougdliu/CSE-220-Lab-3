/*Authhors: Aaron Chasse, Jake Borg, Doug Liu
 *CSE 220 TTH 1030-1145 : Lab 3
 *
 *Program description: This program will be testing the function skip_blanks
*/


#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "print.h"
#include "scanner.h"

//function prototypes
static void skip_blanks(char *ch_ptr1);
//global src_ptr is initialized to NULL
static *src_ptr = NULL;

int main( )
{
	//This will be an invalid parameter since src_ptr is pointing to a Token object, and skip_blanks tries to dereference it
	Token *test_token = malloc(sizeof(Token));
	src_ptr = test_token;
	char ch_test1;
	char string_arr[10] = "      Testing";

	char ch_test2;

    //call skip_blanks to test valid and invalid parameters

    //This will be an invalid parameter but when skip_blanks tries to dereference src_pointer it will be trying
    //to dereference a pointer to a Token struct, which will cause an error
    skip_blanks(ch_test1);
    //valid parameter since ch_test2 and src_ptr both contain valid data
    src_ptr = string_arr;
    ch_test2 = *src_ptr;
    skip_blans( ch_test2 )


    return 0;
}

static void skip_blanks(char *ch_ptr1)
{

	//Move the src_ptr ahead on the source_line to the first nonblank character
	while(*src_ptr == ' ' || *src_ptr == '\t')
	{
		src_ptr++;


	}
	//after iterating through spaces and finding null terminator or a character,
	//set ch to what src_ptr is now pointing at
	*ch_ptr1 = *src_ptr;


}
