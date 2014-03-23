/*Authors: Aaron Chasse, Jake Borg, Doug Liu
 *CSE 220 TTH 1030-1145 : Lab 3
 *
 *Program description: This program will test the function add_token_to_list()
*/


#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "print.h"
#include "scanner.h"

FILE *init_lister(const char *name, char source_file_name[], char date[]);
void quit_scanner(FILE *src_file, Token *list);
void add_token_to_list(Token **head, Token **tail, Token *new_token);

int main(int argc, const char * argv[])
{
	Token *token;
	Token *tail;
	Token *head;
	int i;
	char source_name[MAX_FILE_NAME_LENGTH];
    char date[DATE_STRING_LENGTH];
    FILE *source_file = init_lister(argv[1], source_name, date);
    init_scanner(source_file, source_name, date);
	
	head = NULL;
	tail = NULL;
	token = NULL;
	
	// Valid
	do
    {
        token = get_token();
        add_token_to_list(&head, &tail, token);
        print_token(token);
    }
    while (token->code != 18);

	// Invalid
	/*
	head = NULL;
	tail = NULL;
	for(i = 0; i < 5; i++) {
		token = (Token*)malloc(sizeof(Token));
		token->code = STRING;
		token->type = STRING_LIT;
		token->literal.str_lit = "NOT A DYNAMIC STRING";
		add_token_to_list(&head,&tail,token);
	}
	*/

    quit_scanner(source_file, head);

    return 0;
}
void add_token_to_list(Token **head, Token **tail, Token *new_token)
{
    // Add new_token to the list knowing that list is a linked list.
	if(*head == NULL) {
		*tail = new_token;
		*head = new_token;
	}
	else {
		(*tail)->next = new_token;
		*tail = new_token;
	}
}
void quit_scanner(FILE *src_file, Token *list)
{
    // Stores the next node into a temp, frees the previous, and repeats until the end of the list
    Token* temp = NULL;
    while(list != NULL) {
        temp = list->next;
        
        // Free the token's internally allocated memory for word and/or str_lit based on the token code
        if(list->code == IDENTIFIER) {
            free(list->word);
        }
        else if(list->code == STRING && list->type == STRING_LIT) {
            free(list->literal.str_lit);
        }
        
        free(list); // Free the node
        list = temp;
    }
    fclose(src_file);
}
FILE *init_lister(const char *name, char source_file_name[], char dte[])
{
    time_t timer;
    FILE *file;
    
    strcpy(source_file_name, name);
    file = fopen(source_file_name, "r");
    time(&timer);
    strcpy(dte, asctime(localtime(&timer)));
    return file;
}
