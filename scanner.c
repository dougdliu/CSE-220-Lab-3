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
#include <string.h>
#include "scanner.h"

/*******************
 Static functions needed for the scanner
 You need to design the proper parameter list and
 return types for functions with ???.
 ******************/

static void get_char(char *buffer);
static void skip_comment(char *skip_ptr2, char *line);
static void skip_blanks(char *ch_ptr1);
static int get_word(Token* token );
static void get_number(Token* token);
static void get_string(Token* token);
static int get_special();
static void downshift_word(char *dPtr);
static BOOLEAN is_reserved_word(char const *rPtr, Token* token);


const char* const SYMBOL_STRINGS2[] =
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

	get_char(token_string);

	ch = *src_ptr;
	//if get_char shows us that we have a blank space or the beginning of a comment we need to skip over all spaces and comments
	//until we come to a token
	if(ch == ' ' || ch == '\n' || ch == '{' || ch == '\t')
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
			if(ch == '\n')				
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
		else if(ch == '\n')
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
static void get_char(char* buffer)
{

    /*
     If at the end of the current line (how do you check for that?),
     we should call get source line.  If at the EOF (end of file) we should
     set the character ch to EOF and leave the function.
     */
	//source_buffer to fill by get_source_line
	
	if (src_ptr == NULL )
	{
		//need to pass array to get_source_line to fill
		get_source_line(buffer);
		
		//make src_ptr point to newly filled array from get_source_line
		src_ptr = buffer;
		//set ch to the first character of the new line
		
	}
	else if(*src_ptr == '\n')
	{
		

		get_source_line(buffer);
		src_ptr = buffer;
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
	while(*src_ptr == ' ' || *src_ptr == '\t')
	{
		src_ptr++;

	}
	//after iterating through spaces and finding null terminator or a character,
	//set ch to what src_ptr is now pointing at
	*ch_ptr1 = *src_ptr;

}
static void skip_comment(char *skip_ptr2, char *line)
{
	//ch will come in as beginning bracket {
	//loop until we find an ending bracket
	while(*src_ptr != '}')
	{
		//if we find a newline, call get_char and continue looping
		if(*src_ptr == '\n')
		{
			//pass in the pointer to token_string to get_char to fill the new line
			get_char(line);
		}
		else
		{
		//move the src_pointer ahead on the source_line
		src_ptr++;
	
		}

	}
	//increment src_ptr by 1 to look at what is following the ending bracket
	src_ptr++;

	//make ch in get_token the value of what src_ptr is looking at
	*skip_ptr2 = *src_ptr;

    /*
     Write some code to skip past the comments in the program and return a pointer
     to the first non blank character.  Watch out for the EOF character.
     */
}
static int get_word(Token* token)
{
    /*
     Write some code to Extract the word
     */
	int i;
	CharCode code;
	char ch = *src_ptr;
	char built_word[MAX_SOURCE_LINE_LENGTH] = {""};

	code = char_table[ch];
	
	for(i = 0; (code == LETTER || code == DIGIT); i++)
	{

		built_word[i] = *src_ptr;
		src_ptr++;
		code = char_table[(*src_ptr)];

	}
	built_word[i+1] = '\0';

	


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
static void get_number(Token* token) {
	int i;
	CharCode code;
	char ch = *src_ptr;
	char built_word[MAX_SOURCE_LINE_LENGTH] = {""};

	code = char_table[ch];

	token->code = NUMBER;
	token->type = INTEGER_LIT; // Assume it's an integer for now
	for(i = 0; (code == DIGIT || ch == 'e' || ch == '.'); i++)
	{
		if(ch == 'e' || ch == '.') {
			token->type = REAL_LIT; // If it has an 'e' or a '.', then it's a float
		}
		built_word[i] = *src_ptr;
		src_ptr++;
		code = char_table[(*src_ptr)];
		ch = *src_ptr;
	}
	built_word[i+1] = '\0';

	if(token->type == INTEGER_LIT) {
		token->literal.int_lit = atoi(built_word);
	}
	else if(token->type == REAL_LIT) {
		token->literal.real_lit = atof(built_word);
	}
}
static void get_string(Token* token)
{
    int i;
	char ch = *(src_ptr+ 1);
	char built_word[MAX_SOURCE_LINE_LENGTH] = {""};

	for(i = 0; (ch != '\'' && ch != '\r'); i++)
	{
		src_ptr++;
		built_word[i] = *src_ptr;
		ch = *src_ptr;
	}
	//increment src_ptr to look at next char in line
	src_ptr++;
	built_word[i+1] = '\0';
	token->code = STRING;
	token->type = STRING_LIT;
	token->literal.str_lit = built_word;
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
				puts("gets to here");
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
static BOOLEAN is_reserved_word(const char *rPtr, Token* token )
{

	//printf("%s\n", rw_table[3][0].string);
	//int final = strcmp(rw_table[3][0].string, rw_table[3][0].string);

    /*
     Examine the reserved word table and determine if the function input is a reserved word.
     */
	//printf("%s\n", rPtr);
	//size_t wrd_size = strlen(rPtr);
	int i, j;
	
	char *temp;
	size_t length = strlen(rPtr);
	temp = rw_table[0][0].string;
	i = (int)length;

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
