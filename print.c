//  Names: Aaron Chasse, Jake Borg, Doug Liu
//  CSE 220 - Lab 3
//  print.c
//  Lab3
//
//  Created by Bryce Holton.
//  Copyright (c) 2014 Bryce Holton. All rights reserved.
//
/*   
     Program description: print.c will be used to print the current incoming line from the file
     being read. It will also print a page header when it determines that the line length is at
     the predefined MAX_LINES_PER_PAGE. 
     print_token() will take in a pointer to a token struct and determine what type of data it has,
     then print its value preceded by the appropriate type.


*/

#include "print.h"

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

static void print_page_header(char source_name[], char date[]);

void print_line(char line[], char source_name_to_print[], char date_to_print[])
{
    char save_ch;
    char *save_chp = NULL;
    static int line_count = MAX_LINES_PER_PAGE;
    
    if (++line_count > MAX_LINES_PER_PAGE)
    {
        print_page_header(source_name_to_print, date_to_print);
        line_count = 1;
    }
    if (strlen(line) > MAX_PRINT_LINE_LENGTH) 
    {
        save_chp = &line[MAX_PRINT_LINE_LENGTH];
    }
    if (save_chp)
    {
        save_ch = *save_chp;
        *save_chp = '\0';
    }
    printf("%s", line);
    if (save_chp)
    {
        *save_chp = save_ch;
    }
}
static void print_page_header(char source_name[], char date[])
{
    static int page_number = 0;
    
    putchar(FORM_FEED_CHAR);
    printf("Page    %d  %s  %s\n\n", ++page_number, source_name, date);
}
void print_token(Token *token)
{
    // Checks the code of the token against the enum TokenCode, and prints the appropriate one
    if(token->code == NUMBER) {
        // Decides whether to print an integer or floating point number based on the enum LiteralType
        if(token->type == INTEGER_LIT) {
            printf("     >> %-17s%d\n", SYMBOL_STRINGS[token->code], token->literal.int_lit);
        }
        else if(token->type == REAL_LIT) {
            printf("     >> %-17s%.1e\n", SYMBOL_STRINGS[token->code], token->literal.real_lit);
        }
    }
    else if(token->code == STRING) {
        printf("     >> %-17s%s\n", SYMBOL_STRINGS[token->code], token->literal.str_lit);
    }
    else if( (token->code > 3) && (token->code < 25) )  { // If the code is a symbol
        printf("     >> %-17s%s\n", SYMBOL_STRINGS[token->code], SYMBOL_STRINGS[token->code]);
    }
    else { // The token code is a keyword or an identifier, which are both stored in token->word
        printf("     >> %-17s", SYMBOL_STRINGS[token->code]);
		printf("%s\n",token->word);
    }
}
