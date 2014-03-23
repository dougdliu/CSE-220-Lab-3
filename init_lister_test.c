/*
 * Function test for init_lister()
 */

#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "print.h"
#include "scanner.h"

FILE *init_lister(const char *name, char source_file_name[], char date[]);

int main()
{
	char name1[] = "Filename"; //Valid name
	char name2[] = "WrongFilename"; //Invalid Name
	char filename1[256] = "Filename.txt"; //Valid file Name
	char filename2[256] = "fjjdkfjlsdkfjlksdjlfjksdjfsdlfk"; //Invalid file name
	char date1[26] = "Thu Mar  4 08:51:35 2014"; //Valid date
	char date2[26] = "Tue Mar4 085135 20"; //Invalid date

	init_lister(name1, filename1, date1); //VVV
	init_lister(name1, filename1, date2); //VVI
	init_lister(name1, filename2, date1); //VIV
	init_lister(name1, filename2, date2); //VII
	init_lister(name2, filename1, date1); //IVV
	init_lister(name2, filename1, date2); //IVI
	init_lister(name2, filename2, date1); //IIV
	init_lister(name2, filename2, date2); //III

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
