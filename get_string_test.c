/*
 * Function test for get_string()
 */

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "scanner.h"

static void get_string(Token* token);
static *src_ptr = NULL;

int main()
{
	Token *test = malloc(sizeof(Token));
	char valid_string[14] = "'Test String'";
	src_ptr = valid_string;
	get_string(test);

	src_ptr = test;
	get_string(test);
}

static void get_string(Token* token)
{
    int i;
	char ch = *(src_ptr+ 1);
	char pr = *src_ptr;
	char *built_word = (char*)malloc(sizeof(char)*MAX_SOURCE_LINE_LENGTH);

	src_ptr++;
	for(i = 0; (ch != '\'' || pr == '\\'); i++)
	{
		if(ch == '\'' && pr == '\\') {
			i--;
		}
		built_word[i] = *src_ptr;
		pr = *src_ptr;
		src_ptr++;
		ch = *src_ptr;
	}
	//increment src_ptr to look at next char in line
	src_ptr++;
	built_word[i] = '\0';
	token->code = STRING;
	token->type = STRING_LIT;
	token->literal.str_lit = built_word;
}
