/*Authhors: Aaron Chasse, Jake Borg, Doug Liu
 *CSE 220 TTH 1030-1145 : Lab 3
 *
 *Program description: This program will be testing the function get_char
*/


#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "print.h"
#include "scanner.h"

//function prototypes
static void get_char(char *buffer);
BOOLEAN get_source_line(char source_buffer[]);
//global src_ptr is initialized to NULL
static *src_ptr = NULL;

int main( )
{

	//This is a valid parameter since it is a char array that get_char will fill
    static char token_string1[MAX_TOKEN_STRING_LENGTH];

    //Try to pass test_token into get_char and it will attempt to fill test_token with invalid data
    Token *test_token = malloc(sizeof(Token));
    //call get_char to test valid and invalid parameters

    //valid parameter
    get_char(token_string1);
    //invalid parameter will return error since it tries to fill a token pointer with invalid data
    get_char(test_token);


    return 0;
}

//We need to include function get_source_line since get_char calls it
BOOLEAN get_source_line(char source_buffer[])
{
    char print_buffer[MAX_SOURCE_LINE_LENGTH + 9];
//    char source_buffer[MAX_SOURCE_LINE_LENGTH];  //I've moved this to a function parameter.  Why did I do that?
    static int line_number = 0;


    if (fgets(source_buffer, MAX_SOURCE_LINE_LENGTH, src_file) != NULL)
    {
        ++line_number;
        sprintf(print_buffer, "%4d: %s\n", line_number, source_buffer);
        print_line(print_buffer, src_name, todays_date);
        return (TRUE);
    }
    else
    {
        return (FALSE);
    }
}

//This is the function we are testing
static void get_char(char* buffer)
{

	if (src_ptr == NULL )
	{
		//need to pass array to get_source_line to fill
		get_source_line(buffer);


		//make src_ptr point to newly filled array from get_source_line
		src_ptr = buffer;

	}
	//if src_ptr is looking at the end of the line then call get_source_line to fill token_string
	else if(*src_ptr == '\n' || *src_ptr == '\r')
	{
		get_source_line(buffer);
		src_ptr = buffer;
	}

}//end function get_char



