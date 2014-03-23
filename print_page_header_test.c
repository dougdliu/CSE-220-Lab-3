/*
 * Function test for print_page_header
 */

#include "print.h"

static void print_page_header(char source_name[], char date[]);

int main()
{
	char test_name1[256] = "TestFile.pas"; //valid file name
	char test_name2[256] = "TestFile.jpg"; //Invalid file name
	char date1[256] = "Thu Mar  4 08:51:35 2014"; //Valid date
	char date2[256] = "Tue Mar4 085135 20"; //Invalid date
	
	print_page_header(char test_name1[], char date1[]); //VV
	print_page_header(char test_name1[], char date2[]); //VI
	print_page_header(char test_name2[], char date1[]); //IV
	print_page_header(char test_name2[], char date2[]); //II
}

static void print_page_header(char source_name[], char date[])
{
    static int page_number = 0;

    putchar(FORM_FEED_CHAR);
    printf("Page    %d  %s  %s\n\n", ++page_number, source_name, date);
}
