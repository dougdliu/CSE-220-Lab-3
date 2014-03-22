/*Authhors: Aaron Chasse, Jake Borg, Doug Liu
 *CSE 220 TTH 1030-1145 : Lab 3
 *
 *Program description: This program will be testing the function get_special
*/


#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "print.h"
#include "scanner.h"

//function prototypes
static int get_special();
//global src_ptr is initialized to NULL
static *src_ptr = NULL;

int main( )
{
	Token *token_test = malloc(sizeof(Token));
	// get_special has no parameters so tests will be conducted global var src_ptr looking at valid or invalid
	// data

	//this is a valid test case since src_ptr is pointing to a valid string array
	char string[5] = "test";
	src_ptr = string;
	get_special();

	//this is an invalid test case since src_ptr is pointing to invalid data for dereferencing
	src_ptr = token_test;
	get_special();

    return 0;
}


static int get_special()
{


	int symbol_code;
	int i;	//counter
	char check;
	//char *test;
	char current_char = *src_ptr;
	char storage[2];
	char *ptr = storage;


	storage[0] = *src_ptr;
	storage[1] = '\0';


	for(i=4;i<=19;i++)
	{
		//test = SYMBOL_STRINGS2[i];
		//printf("%s\n", ptr);
		if(strcmp(ptr, SYMBOL_STRINGS2[i]) == 0)
		{
			check = *(src_ptr + 1);


			//printf("%c\n", current_char);
			//printf("%c\n", *src_ptr);
			if(check == '=' && current_char == ':')
			{
				symbol_code = 20;
				src_ptr += 2;
				return symbol_code;
			}
			else if(check == '=' && current_char == '<')
			{
				symbol_code = 21;
				src_ptr += 2;
				return symbol_code;
			}
			else if(check == '=' && current_char == '>')
			{
				symbol_code = 22;
				src_ptr += 2;
				return symbol_code;
			}
			else if(check == '>' && current_char == '<')
			{
				symbol_code = 23;
				src_ptr += 2;
				return symbol_code;
			}
			else if(check == '.' && current_char == '.')
			{
				symbol_code = 24;
				src_ptr += 2;
				return symbol_code;
			}
			else
			{
				src_ptr++;
				symbol_code = i;
				return symbol_code;
			}
		}
		//if no match for loop will continue to loop
	}//end for loop


}
