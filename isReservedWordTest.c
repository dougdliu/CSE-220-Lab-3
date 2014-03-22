/*Authhors: Aaron Chasse, Jake Borg, Doug Liu
 *CSE 220 TTH 1030-1145 : Lab 3
 *
 *Program description: This program will be testing the function is_reserved_word
*/


#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "print.h"
#include "scanner.h"

//function prototypes
static BOOLEAN is_reserved_word(const char *rPtr, Token* token )
//global src_ptr is initialized to NULL
static *src_ptr = NULL;

int main( )
{
	Token *test_token1 = malloc(sizeof(Token));
	char string1[5] = "test";
	int tester = 5;

	//(valid, valid)
	is_reserved_word(string1, test_token1);
	//(valid, invalid)
	is_reserved_word(string1, tester);
	//(invalid, valid)
	is_reserved_word(test_token1, test_token1);
	//(invalid, invalid)
	is_reserved_word(tester, tester);


    return 0;
}

static BOOLEAN is_reserved_word(const char *rPtr, Token* token )
{

	int i, j;

	char *temp;
	size_t length = strlen(rPtr);
	temp = rw_table[0][0].string;
	i = (int)length;

	/*
	     Examine the reserved word table and determine if the function input is a reserved word.
     */
	for(j = 0; j <= 10; j++)
	{
		temp = rw_table[(i-2)][j].string ;
		if(temp == NULL)
		{
			return FALSE;
		}
		if(strcmp(rPtr, rw_table[(i-2)][j].string) == 0)
		{
			token->code = rw_table[(i-2)][j].token_code;
			return TRUE;
		}
	}


}
