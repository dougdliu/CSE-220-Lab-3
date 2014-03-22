//
//  main.c
//  Lab3
//
//  Created by Bryce Holton.
//  Copyright (c) 2014 Bryce Holton. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "print.h"
#include "scanner.h"

FILE *init_lister(const char *name, char source_file_name[], char date[]);
void quit_scanner(FILE *src_file, Token *list);
void add_token_to_list(Token **sPtr, Token *new_token);

int main(int argc, const char * argv[])
{
    Token *token;
    Token *token_list = NULL; //This needs to be implemented as a linked list in scanner.h.
    
    char source_name[MAX_FILE_NAME_LENGTH];
    char date[DATE_STRING_LENGTH];
    FILE *source_file = init_lister(argv[1], source_name, date);
    init_scanner(source_file, source_name, date);
    
    do
    {
        token = get_token();
        add_token_to_list(&token_list, token);
        print_token(token);
    }
    while (token->code != 18);//What is the sentinal value that ends this loop?
    //if the token code is special character '.' then it is the end of file
    //and the symbol_code will be 19
    
    quit_scanner(source_file, token_list);
    return 0;
}
void add_token_to_list(Token **sPtr, Token *new_token)
{
    // Add new_token to the list knowing that list is a linked list.
    
    // Iterates to the last token in the list (by seeing when token->next == NULL), 
    // then sets the next to new_tokwn
	
	
	Token *currentPtr;
	//if the currentPtr is null, then point it to the new_token coming in
	if(*sPtr == NULL) {
		
		currentPtr = new_token;
		//point the star_ptr to the new_token this is the beginning of the list
		(*sPtr) = new_token;
		(*sPtr)->nxt = NULL;
	}
	else {
		//if current_ptr is not null, say that the next token is the incoming token
		currentPtr->next = new_token;
		//then point current_ptr to the incoming token, this way the previous token will be pointing
		//to the newest token in the list
		currentPtr = new_token;
		currentPtr->next = NULL;
	}
}
void quit_scanner(FILE *src_file, Token *list)
{
    // Stores the next node into a temp, frees the previous, and repeats until the end of the list
    Token* temp = NULL;
    while(list != NULL) {
        temp = list->next;
        
        // Free the token's internally allocated memory for word and/or str_lit based on the token code
        /*
        if(list->code == IDENTIFIER) {
            free(list->word);
        }
        else if(list->code == STRING && list->type == STRING_LIT) {
            free(list->literal.str_lit);
        }
        */
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

