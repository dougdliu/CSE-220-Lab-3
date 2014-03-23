/*
 * Function Test for print_line
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

int main()
{
	char testname1[] = "TestFile.pas"; //Valid file name
	char testname2[] = "TestFile.jpg"; //Invalid file name
	char testdate1[] = "Thu Mar  4 08:51:35 2014"; //Valid date
	char testdate2[] = "Tue Mar4 085135 20"; //Invalid date
	char testline1[] = "This is a test line"; //valid test line
	char testline2[] = "sdflkjsldkjflsjdfksdfoieieoieoweoeijowejfiojeflwjeflksdjflkagamvmavlkamvksmlsldkmslkdmscscsxcscxsxcsxcscxsklcxmlskxcklsmcxlkmsldkdfjlkajg9pwuertqa"; //invalid test line

	print_line(testname1, testdate1, testline1); //VVV
	print_line(testname1, testdate1, testline2); //VVI
	print_line(testname1, testdate2, testline1); //VIV
	print_line(testname1, testdate2, testline2); //VII
	print_line(testname2, testdate1, testline1); //IVV
	print_line(testname2, testdate1, testline2); //IVI
	print_line(testname2, testdate2, testline1); //IIV
	print_line(testname2, testdate2, testline2); //III
}


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
