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
		return -1;
}

//multi-char input
char* toupper(char* string) {
	char* newString = sys_alloc_mem(100);
	//index variable
	int i = 0;
	//until hitting null-termination symbol, cycle through the indexes of the string
	//for each index, if a lowercase char, will make uppercase, then put in new string.
	while (string[i] != '\0') {
		if (string[i] >= 97 && string[i] <= 122)
			newString[i] = (((string[i]) - 32));
		else
			newString[i] = string[i];
		i++;
	}
	return newString;
		
}
