#include <ctype.h>
#include <string.h>
#include <memory.h>

int isspace(int c)
{
	return (c == ' ' || c == '\n' || c == '\r' || c == '\f' || c == '\t' || c == '\v');
}
int isdigit(int c){
	if (c >= 48 && c <= 57)
		return (c >= 48 && c <= 57);
	else
		return 0;
}

//multi-char input
int toupper(char c) {
	//index variable
	//until hitting null-termination symbol, cycle through the indexes of the string
	//for each index, if a lowercase char, will make uppercase, then put in new string.

		if (c >= 97 && c <= 122)
			return c-32;
		else
			return c;

		
}
