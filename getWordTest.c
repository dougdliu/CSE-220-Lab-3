/*Authhors: Aaron Chasse, Jake Borg, Doug Liu
 *CSE 220 TTH 1030-1145 : Lab 3
 *
 *Program description: This program will be testing the function get_word
*/


#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "print.h"
#include "scanner.h"

//function prototypes
static int get_word(Token* token );
//global src_ptr is initialized to NULL
static *src_ptr = NULL;

int main( )
{
	Token *test_token = malloc(sizeof(Token));

	//This will be part of the valid parameter, since src_ptr will be pointing to a valid char array
	char valid_string[6] = "array";
	src_ptr = valid_string;

	//call get_word on the token with src_ptr pointing to valid data
	get_word(test_token);

	/*Now point src_ptr to an invalid data and attempt to call get_word
	  since src_ptr is pointing at the token being built get_word will have an
	  error when it attempts to get the current character
	*/
	src_ptr = test_token;
	get_word(test_token);


    return 0;
}


static int get_word(Token* token)
{
	//declare and initialize function variables
	int i;
	CharCode code;
	char ch = *src_ptr;
	//allocate memory for the built_word
	char *built_word = (char*)malloc(sizeof(char)*MAX_SOURCE_LINE_LENGTH);

	//check to code for ch in the char_table
	code = char_table[ch];

	//iterate through string to build the word
	for(i = 0; (code == LETTER || code == DIGIT); i++)
	{
		built_word[i] = *src_ptr;
		src_ptr++;
		code = char_table[(*src_ptr)];


	}
	built_word[i] = '\0';


    //Downshift the word, to make it lower case
	downshift_word(built_word); //This downshifts the word


    /*
     Write some code to Check if the word is a reserved word.
     if it is not a reserved word its an identifier.
     */
	if(strlen(built_word) > 1)
	{


		if((is_reserved_word(built_word, token)) == FALSE)//checks to see if the condition is false
		{
			token->code = IDENTIFIER;
		}
	}
	else
	{
		token->code = IDENTIFIER;
	}
	token->word = built_word; //assigns the token to be an identifier.
	return 0;
}
