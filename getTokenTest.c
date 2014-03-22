/*Authhors: Aaron Chasse, Jake Borg, Doug Liu
 *CSE 220 TTH 1030-1145 : Lab 3
 *
 *Program description: This program will be testing the function get_Token
*/


#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "print.h"
#include "scanner.h"

//global src_ptr is initialized to NULL
static *src_ptr = NULL;

int main( )
{
	Token *token;
	char string1;
	//valid
	token = get_token();
	//invalid: trying to assign a pointer to Token struct to a char
	string1 = get_token();

    return 0;
}

Token* get_token()
{
        char ch; //This can be the current character you are examining during scanning.
	static char token_string[MAX_TOKEN_STRING_LENGTH]; //Store your token here as you build it.
	char *token_ptr = token_string; //write some code to point this to the beginning of token_string
	int loop = FALSE;
	int symbol_code;
	Token* token = (Token*)malloc(sizeof(Token)); //allocate memory for struct


	CharCode code;


	//get_char will set global ptr src_ptr to the source_buffer line
	//get_char will also set ch to the first character in source_buffer, if the end of line has been reached,
	//otherwise will set ch to what GLOBAL src_ptr is currently looking at.
	//other methods will set ch to the next char in the source_buffer after they have tokenized
	token->next = NULL;
	get_char(token_string);


	ch = *src_ptr;
	//if get_char shows us that we have a blank space or the beginning of a comment we need to skip over all spaces and comments
	//until we come to a token
	if(ch == ' ' || ch == '\n' || ch == '{' || ch == '\r' || ch == '\t')
	{
		loop = TRUE; // Execute the loop for skipping spaces, skipping comments, and getting a new line
	}
	while(loop)
	{


		//check to see what the current ch is
		if(ch == ' ' || ch == '\t')
		{
			//call function skip_blanks which returns a pointer to the first non-blank character				// if it reaches null terminator it will set ch to '\0' and come back here


			skip_blanks(&ch);


			//now call get_char again to get a new line if ch is a null terminator
			if(ch == '\n' || ch == '\r')
			{


				//call get_char to get a new source line
				get_char(token_string);
				ch = *src_ptr;
			}
			//after this there may be more spaces or comments, need to continue to evaluae loop
			loop = TRUE;
		}
		//now check to see if there is a comment and skip over this as well
		else if(ch == '{')
		{
			loop = TRUE;
			//skip_comment will return the character following the ending bracket
			//src_ptr will also be pointing here
			//ch will contin current value of src_ptr
			skip_comment(&ch, token_string);


		}
		else if(ch == '\n' || ch == '\r')
		{
			get_char(token_string);
			ch = *src_ptr;
			loop = TRUE;
		}
		else
		{
			//if no matches then we are looking at a valid character
			loop = FALSE;
			//make sure to update ch to what src_ptr is looking at
			ch = *src_ptr;
		}


	}

    //position 'ch' in char_table will return a CharCode corresponding to Letter, Digit, Quote, etc...
	code = char_table[ch];
	//check to see if code for ch is a LETTER
	if(code == LETTER)
	{
		//pass in the token_string array to point to, and the current Token struct.
		//get_word will set the tokens values appropriately so it can be returned
		//to main
		get_word( token);
	}
	//check to see if it is a digit
	else if(code == DIGIT)
	{
		get_number(token); //The parameter has to be same as get_word because we need to build a character array to be converted to integers or real numbers
	}
	//check to see if it is a quote
	else if(code == QUOTE)
	{
		get_string(token);
	}
	else
	{
		//in the get_special function the token code will be set
		symbol_code = get_special();
		//set token code to the symbol code
		token->code = (TokenCode)symbol_code;
		//the literal type for the token will be a str_lit and the char ptr will point to the token_string array where
		//the characters are stored
		token->literal.str_lit = token_string;
		//type of token will be set to string_lit
		token->type = STRING_LIT;


	}
    //3.  Call the appropriate function to deal with the cases in 2.


   return token; //What should be returned here?
}
