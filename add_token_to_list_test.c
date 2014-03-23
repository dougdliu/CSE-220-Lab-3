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

void add_token_to_list(Token **head, Token **tail, Token *new_token);

int main( )
{
	Token *token;
	Token *tail;
	Token *head;
	
	//valid, valid, valid
	*head = (Token*)malloc(sizeof(Token));
	*tail = (Token*)malloc(sizeof(Token));
	*token = (Token*)malloc(sizeof(Token));
	//v,v,i
	/**
	*head = (Token*)malloc(sizeof(Token));
	*tail = (Token*)malloc(sizeof(Token));
	*token = (Token*)malloc(sizeof(Token));
	 */
	//v,i,v
	/**
	*head = (Token*)malloc(sizeof(Token));
	*tail = (Token*)malloc(sizeof(Token));
	*token = (Token*)malloc(sizeof(Token));
	 */
	//v,i,i
	/**
	*head = (Token*)malloc(sizeof(Token));
	*tail = (Token*)malloc(sizeof(Token));
	*token = (Token*)malloc(sizeof(Token));
	 */
	//i,v,v
	/**
	*head = (Token*)malloc(sizeof(Token));
	*tail = (Token*)malloc(sizeof(Token));
	*token = (Token*)malloc(sizeof(Token));
	 */
	//i,v,i
	/**
	*head = (Token*)malloc(sizeof(Token));
	*tail = (Token*)malloc(sizeof(Token));
	*token = (Token*)malloc(sizeof(Token));
	 */
	//i,i,v
	/**
	*head = (Token*)malloc(sizeof(Token));
	*tail = (Token*)malloc(sizeof(Token));
	*token = (Token*)malloc(sizeof(Token));
	 */
	//i,i,i
	/**
	*head = (Token*)malloc(sizeof(Token));
	*tail = (Token*)malloc(sizeof(Token));
	*token = (Token*)malloc(sizeof(Token));
	 */
	add_token_to_list(&head,&tail,token);

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
