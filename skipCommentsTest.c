/*Authhors: Aaron Chasse, Jake Borg, Doug Liu
 *CSE 220 TTH 1030-1145 : Lab 3
 *
 *Program description: This program will be testing the function skip_comment
*/


#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "print.h"
#include "scanner.h"

//function prototypes
static void skip_comment(char *skip_ptr2, char *line);
//global src_ptr is initialized to NULL
static *src_ptr = NULL;

int main( )
{
	Token *test_token = malloc(sizeof(Token));
	char string1[9] = "{tester}";
	char string2[8] = "invalid";
	char ch1, ch2, ch3;



	//(valid, valid)
	skip_comment(ch1, string1);
	//(valid, invalid)
	skip_comment(ch2, test_token);
	//(invalid, valid)
	skip_comment(test_token, string1);
	//(invalid, valid)
	skip_comment(string2, test_token);

    return 0;
}

static void skip_comment(char *skip_ptr2, char *line)
{
	//ch will come in as beginning bracket {
	//loop until we find an ending bracket
	while(*src_ptr != '}')
	{
		//if we find a newline, call get_char and continue looping
		if(*src_ptr == '\n' || *src_ptr == '\r')
		{
			//pass in the pointer to token_string to get_char to fill the new line
			get_char(line);
		}
		else
		{
			//move the src_pointer ahead on the source_line
			src_ptr++;
		}


	}//end while loop

	//increment src_ptr by 1 to look at what is following the ending bracket
	src_ptr++;

	//make ch in get_token the value of what src_ptr is looking at
	*skip_ptr2 = *src_ptr;

}
