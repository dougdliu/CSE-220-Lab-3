//
//  print.c
//  Lab3
//
//  Created by Bryce Holton.
//  Copyright (c) 2014 Bryce Holton. All rights reserved.
//

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
    if(token->code == IDENTIFIER) {
        printf("     >> %17s%s", SYMBOL_STRINGS[token->code], token->identifier);
    }
    else if(token->code == NUMBER) {
        // Decides whether to print an integer or floating point number based on the enum LiteralType
        if(token->type == INTEGER_LIT) {
            printf("     >> %17s%d", SYMBOL_STRINGS[token->code], token->literal.int_lit);
        }
        else if(token->type == REAL_LIT) {
            printf("     >> %17s%f", SYMBOL_STRINGS[token->code], token->literal.real_lit);
        }
    }
    else if(token->code == STRING) {
        printf("     >> %17s%s", SYMBOL_STRINGS[token->code], token->literal.str_lit);
    }
    else {
        printf("     >> %17s%s", SYMBOL_STRINGS[token->code], downshift_word(SYMBOL_STRINGS[token->code]));
    }
}
%s
