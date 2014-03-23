/*
 * Function test for get_number()
 */


#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "print.h"
#include "scanner.h"
#include "scanner.c"

static int get_number(Token *token);
static *src_ptr = NULL;

int main()
{
	Token *test =  malloc(sizeof(Token));
	char valid_num[10] = "-1.54e-07";
	src_ptr = valid_num;
	get_number(test);

	src_ptr = test;
	get_number(test);
}

static void get_number(Token* token) {
	int i;
	CharCode code;
	char ch = *src_ptr;
	char *built_word = (char*)malloc(sizeof(char)*MAX_SOURCE_LINE_LENGTH);

	code = char_table[ch];

	token->code = NUMBER;
	token->type = INTEGER_LIT; // Assume it's an integer for now
	for(i = 0; (code == DIGIT || ch == 'e' || ch == '.' || ch == '-'); i++)
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
	free(built_word);
}
