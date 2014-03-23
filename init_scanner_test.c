/*
 * Function test for init_scanner
 */

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "scanner.h"
#include "common.h"

/*******************
 Static functions needed for the scanner
 You need to design the proper parameter list and
 return types for functions with ???.
 ******************/

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

int main()
{
	FILE test_file1 = fopen("Testcases1.pas", "r"); //Valid file
	FILE test_file2 = fopen("Test.jpg"); //Invalid file
	char test_name1[256] = "Testcases1.pas"; //Valid name
	char test_name2[256] = "TestCaseThatIsTooLongForThisToReallyWorkAsItIsGoingToExeedTheAmountOfCharacterThatWeAreAllowedToHaveALKJLKJDSLFKJSDKFJLSDJFLKSDJLFKJLKSDJLFKJSLDKJFLSLKDJFLKJSASDASDASDASDFFFEFEEFEFEFQWDSDQWDQWDASD.PAS"; //Invalid name
	char test_date1[DATE_STRING_LENGTH]; //Valid date
	char test_date2[DATE_STRING_LENGTH]; //Invalid date

	void init_scanner(test_file1, test_name1, test_date1); //VVV
	void init_scanner(test_file1, test_name1, test_date2); //VVI
	void init_scanner(test_file1, test_name2, test_date1); //VIV
	void init_scanner(test_file1, test_name2, test_date2); //VII
	void init_scanner(test_file2, test_name1, test_date1); //IVV
	void init_scanner(test_file2, test_name1, test_date2); //IVI
	void init_scanner(test_file2, test_name2, test_date1); //IIV
	void init_scanner(test_file2, test_name2, test_date2); //III

}

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
