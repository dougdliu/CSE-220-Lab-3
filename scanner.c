//
//  scanner.c
//  Lab3
//
//  Created by Bryce Holton.
//  Copyright (c) 2014 Bryce Holton. All rights reserved.
//

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "scanner.h"

/*******************
 Static functions needed for the scanner
 You need to design the proper parameter list and
 return types for functions with ???.
 ******************/

static void get_char(char *ch_ptr2);
static char skip_comment(char current_ch );
static void skip_blanks(char *ch_ptr1);
static void get_word(char string[], Token* token );
static void get_number(char* ch, Token* token) {
static void get_string(char* ch, Token* token );
static int get_special();
static void downshift_word(char *dPtr);
static BOOLEAN is_reserved_word(char const *rPtr);


const char* const SYMBOL_STRINGS[] =
{
    "<no token>", "<IDENTIFIER>", "<NUMBER>", "<STRING>",
    "^","*","(",")","-","+","=","[","]",":",";",
    "<",">",",",".","/",":=","<=",">=","<>","..",
    "<END OF FILE>", "<ERROR>",
    "AND","ARRAY","BEGIN","CASE","CONST","DIV","DO","DOWNTO",
    "ELSE","END","FILE","FOR","FUNCTION","GOTO","IF","IN",
    "LABEL","MOD","NIL","NOT","OF","OR","PACKED","PROCEDURE",
    "PROGRAM","RECORD","REPEAT","SET","THEN","TO","TYPE","UNTIL",
    "VAR","WHILE","WITH",
};



typedef enum
{
    LETTER, DIGIT, QUOTE, SPECIAL, EOF_CODE,
}
CharCode;

/*********************
 Static Variables for Scanner
 Must be initialized in the init_scanner function.
 *********************/
static FILE *src_file;
static char src_name[MAX_FILE_NAME_LENGTH];
static char todays_date[DATE_STRING_LENGTH];
static CharCode char_table[256];  // The character table

//Global pointer for pointing to current line being tokenized
static char* src_ptr = NULL;

typedef struct
{
    char *string;
    TokenCode token_code;
}
RwStruct;

const RwStruct rw_table[9][10] = {
    {{"do",DO},{"if",IF},{"in",IN},{"of",OF},{"or",OR},{"to",TO},{NULL,0}}, //Reserved words of size 2
    {{"and",AND},{"div",DIV},{"end",END},{"for",FOR},{"mod",MOD},{"nil",NIL},{"not",NOT},{"set",SET},{"var",VAR},{NULL,0}}, //Reserved words of size 3
    {{"case",CASE},{"else",ELSE},{"file",FFILE},{"goto",GOTO},{"then",THEN},{"type",TYPE},{"with",WITH},{NULL,0}}, //Reserved words of size 4
    {{"array",ARRAY},{"begin",BEGIN},{"const",CONST},{"label",LABEL},{"until",UNTIL},{"while",WHILE},{NULL,0}},  //Reserved words of size 5
    {{"downto",DOWNTO}, {"packed",PACKED},{"record",RECORD}, {"repeat",REPEAT},{NULL,0}},  // Reserved words of size 6
    {{"program", PROGRAM},{NULL,0}}, // Reserved words of size 7
    {{"function", FUNCTION},{NULL,0}}, // Reserved words of size 8
    {{"procedure", PROCEDURE},{NULL,0}}  // Reserved words of size 9
};

void init_scanner(FILE *source_file, char source_name[], char date[])
{
    int i;
    src_file = source_file;
    strcpy(src_name, source_name);
    strcpy(todays_date, date);

    /*******************
     initialize character table, this table is useful for identifying what type of character
     we are looking at by setting our array up to be a copy the ascii table.  Since C thinks of
     a char as like an int you can use ch in get_token as an index into the table.
     *******************/
    for(i = 0; i < 256; i++) {
    	// Assign the letters
    	if( (i > 64 && i < 91) || (i > 96 && i < 123) ) {
    		char_table[i] = LETTER;
    	}
    	// Assign the digits
    	else if(i > 47 && i < 58) {
    		char_table[i] = DIGIT;
    	}
    	// Everything else is a special
    	else {
    		char_table[i] = SPECIAL;
    	}
    }
    // Then rewrite the two execptions, the quotation mark and the period
    char_table[39] = QUOTE; // Pascal uses single quotes
    char_table[46] = EOF_CODE;
}
BOOLEAN get_source_line(char source_buffer[])
{
    char print_buffer[MAX_SOURCE_LINE_LENGTH + 9];
//    char source_buffer[MAX_SOURCE_LINE_LENGTH];  //I've moved this to a function parameter.  Why did I do that?
    static int line_number = 0;

    if (fgets(source_buffer, MAX_SOURCE_LINE_LENGTH, src_file) != NULL)
    {
        ++line_number;
        sprintf(print_buffer, "%4d: %s", line_number, source_buffer);
        print_line(print_buffer, src_name, todays_date);
        return (TRUE);
    }
    else
    {
        return (FALSE);
    }
}
Token* get_token()
{
        char ch; //This can be the current character you are examining during scanning.
	char token_string[MAX_TOKEN_STRING_LENGTH]; //Store your token here as you build it.
	char *token_ptr = token_string; //write some code to point this to the beginning of token_string
	int loop = FALSE;
	int symbol_code;
	Token* token = (Token*)malloc(sizeof(Token)); //allocate memory for struct
	
	CharCode code;

	//get_char will set global ptr src_ptr to the source_buffer line
	//get_char will also set ch to the first character in source_buffer, if the end of line has been reached,
	//otherwise will set ch to what GLOBAL src_ptr is currently looking at.
	//other methods will set ch to the next char in the source_buffer after they have tokenized
	ch = *src_ptr;
	get_char(&ch);
    //1.  Skip past all of the blanks


	//if get_char shows us that we have a blank space or the beginning of a comment we need to skip over all spaces and comments
	//until we come to a token
	do
	{
		if(ch == ' ')
		{
			//call function skip_blanks which returns a pointer to the first non-blank character
			// if it reaches null terminator it will set ch to '\0' and come back here
			skip_blanks(&ch);
			//now call get_char again to get a new line if ch is a null terminator
			if(ch == '\0')
			{
				//There may be more spaces, need to continue to evaluate
				loop = TRUE;
				//call get_char to get a new source line
				get_char(&ch);
			}
			//if the current char being looked at is not a null terminator, go on
			//to see if it is a comment. If not a comment we will loop to make sure it's not another space
		}
		//now check to see if there is a comment and skip over this as well
		if(ch == '{')
		{
			loop = TRUE;
			//skip_comment will return the character following the ending bracket
			//src_ptr will also be pointing here
			//it contains a loop to skip over all white spaces and chars within the comment
			ch = skip_comment(ch);

			//Now call get_char on ch to see what we are looking at (could be another comment or more spaces,
			//so we cannot end loop yet).
			get_char(&ch);
		}
		else
		{
			loop = FALSE;
		}

	}while(loop);

	//after this do-while loop we know that we are looking at something that is not a space or a comment block

    //2.  figure out which case you are dealing with LETTER, DIGIT, QUOTE, EOF, or special, by examining ch

    //position 'ch' in char_table will return a CharCode corresponding to Letter, Digit, Quote, etc...
	code = char_table[ch];
	//check to see if code for ch is a LETTER
	if(code == LETTER)
	{
		//pass in the token_string array to point to, and the current Token struct.
		//get_word will set the tokens values appropriately so it can be returned
		//to main
		get_word(token_string, token);
	}
	//check to see if it is a digit
	else if(code == DIGIT)
	{
		get_number(&ch, token); //The parameter has to be same as get_word because we need to build a character array to be converted to integers or real numbers
	}
	//check to see if it is a quote
	else if(code == QUOTE)
	{
		get_string(&ch, token);
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
static void get_char(char* ch_ptr2)
{
    /*
     If at the end of the current line (how do you check for that?),
     we should call get source line.  If at the EOF (end of file) we should
     set the character ch to EOF and leave the function.
     */
	char source_buffer[MAX_SOURCE_LINE_LENGTH];	
	
	if (*ch_ptr2 == '\0' || ch_ptr2 == NULL )
	{
		//need to pass array to get_source_line to fill
		get_source_line(source_buffer);
		//make src_ptr point to newly filled array from get_source_line
		src_ptr = source_buffer;
		//set ch to the first character of the new line
		*ch_ptr2 = source_buffer[0];
	}
	else
	{
		//otherwise ch will just be set to what src_ptr is currently at
		*ch_ptr2 = *src_ptr;
	}
}
static void skip_blanks(char *ch_ptr1)
{
    /*
     Write some code to skip past the blanks in the program and return a pointer
     to the first non blank character
     */
	//Move the src_ptr ahead on the source_line to the first nonblank character
	//if it finds a ull terminator it will break out of while loop
	//and ch will be set to the null terminator, otherwise it will be set
	//to whatever character src_ptr is now looking at
	while(*src_ptr == ' ' && *src_ptr != '\0')
	{
		src_ptr++;
		
	}
	//after iterating through spaces and finding null terminator or a character,
	//set ch to what src_ptr is now pointing at
	*ch_ptr1 = *src_ptr;

}
static char skip_comment(char current_ch)
{
	//ch will come in as beginning bracket {
	while(current_ch != '}')
	{
		//if current_ch
		if(current_ch == '\0')
		{
			get_char(&current_ch);
		}
		else
		{
		//move the src_pointer ahead on the source_line
		src_ptr++;
		current_ch = *src_ptr;
		}

	}
	//increment src_ptr by 1 to look at what is following the ending bracket
	src_ptr++;

	current_ch = *src_ptr;
	return current_ch;

    /*
     Write some code to skip past the comments in the program and return a pointer
     to the first non blank character.  Watch out for the EOF character.
     */
}
static void get_word(char string[], Token* token)
{
    /*
     Write some code to Extract the word
     */
	size_t i = 0;
	char word[MAX_TOKEN_STRING_LENGTH];
	while(string[i] != '\0') //While string hasn't reached EOF of the token string copy string into word
	{
		if(string[i] == LETTER) //If it's a letter, it keeps going
		{
			word[i] = string[i];
			i++;
		}
		
		else if(string[i] == DIGIT) //If it's a number, it still keeps going, but it checks if there is a letter first so that it doesn't confuse it with a number
		{
			word[i] = string[i];
			i++;
		}
		
		else if(string[i] == SPECIAL) //If it's a special it ends the loop
		{
			break;
		}
		
		else if(string[i] == ' ') //If it's a blank space, it breaks the loop
		{
			break;
		}
		
		else if(string[i] == QUOTE) //If it's a quote, it also breaks the loop
		{
			break;
		}
	}
    //Downshift the word, to make it lower case
	downshift_word(word); //This downshifts the word
    /*
     Write some code to Check if the word is a reserved word.
     if it is not a reserved word its an identifier.
     */
	if((is_reserved_word(word)) == FALSE){ //checks to see if the condition is false
		token->word = word; //assigns the token to be an identifier.
	}
}
static void get_number(char* ch, Token* token) {
	int i;
	char *end, *temp;
	token->type = INTEGER_LIT; // Assume it's an integer for now
	for(end = ch; char_table[*end] == DIGIT || *end == 'e' || *end == '.'; end++) {} // Find the end of the number so we know the size
	temp = (char*)malloc(end - ch); // Allocate memory to the temp based on that size
	for(i = 0; i <= (end - ch); i++) { // For the whole number
        *(temp + i) = *(ch + i); // Copy the number into the temp

		if(*end == 'e' || *end == '.') { // If there's an 'e', or '.', we change the type to float
		    token->type = REAL_LIT; // Make the type a float
		}
	}
	if(token->type == INTEGER_LIT) { // If it's an integer
		token->literal.int_lit = atoi(temp); // Parse the string into an integer. atoi() takes a string and returns an integer
	}
	else if(token->type == INTEGER_LIT) { // If it's a floating point number
		token->literal.real_lit = atof(temp); // Parse the string into an double. atoi() takes a string and returns an double
	}
	src_ptr = end + 1; // Adjust the global pointer to after the string
	*ch = *(end + 1); // Set ch to the char after the string ends
	free(temp); // BE FREE
}
static void get_string(char* ch, Token* token)
{
    // would this function need to have Token **token as the argument since it is receiving a pointer?
    int i;
    char* iterator; // Temp location in string
    token->type = STRING_LIT; // Change the type of the literal to String
    for(iterator = ch + 1; *iterator != '\'' && *(iterator - 1) != '\\'; iterator++) {} // Find the end of the string, used for finding the size. Skip over escape characters
    token->literal.str_lit = (char*)malloc(iterator - ch); // Allocate memory based on that size
    for(i = 0; i < (iterator - ch - 1); i++) { // Iterate again through string
        *(token->literal.str_lit + i) = *(ch + i + 1); // Copy the contents of the string to the literal in the Token
    }
    *(token->literal.str_lit + (iterator - ch - 1)) = '\0'; // Add a null terminator
    src_ptr = iterator + 1; // Adjust the global pointer to after the string
    *ch = *(iterator + 1); // Set ch to the char after the string ends
}
static int get_special()
{
	
	int symbol_code;
	int i;	//counter
	char check;	

	for(i=4;i<=19;i++)
	{
		if(strcmp(src_ptr, SYMBOL_STRINGS[i]) == 0)
		{
			check = *(src_ptr + 1);
			
			if(check == '=' && *src_ptr == ':')
			{
				symbol_code = 20;
				src_ptr += 2;
				return symbol_code;
			}
			else if(check == '=' && *src_ptr == '<')
			{
				symbol_code = 21;
				src_ptr += 2;
				return symbol_code;
			}
			else if(check == '=' && *src_ptr == '>')
			{
				symbol_code = 22;
				src_ptr += 2;
				return symbol_code;
			}
			else if(check == '>' && *src_ptr == '<')
			{
				symbol_code = 23;
				src_ptr += 2;
				return symbol_code;
			}
			else if(check == '.' && *src_ptr == '.')
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
static BOOLEAN is_reserved_word(char const *rPtr)
{
    /*
     Examine the reserved word table and determine if the function input is a reserved word.
     */
	int i, j; // counters for for loops
	for(i = 0; i <= 9; i++)
	{
		for(j = 0; j <= 10; j++)
		{
			
			//compare the string pointed to be *rPtr to reserved words in rw_table
			//if match is found return true
			if(strcmp(rPtr, rw_table[i][j].string) == 0)
			{
				return TRUE;
			}
		}
	}
    return FALSE;
}
