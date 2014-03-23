/*
 * Function test for print_token()
 */


#include "print.h";

void print_token(Token *token);
static *src_ptr = NULL;

int main(){
	Token *token;
	char string[] = "Hello";
	int n = 1;
	float f = 1.0101;

	token->word = string;
	token->literal.int_lit = n;
	token->literal.real_lit = f;
	
	print_token(token); //V
	print_token(*token); //I

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
