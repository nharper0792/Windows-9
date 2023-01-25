#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
void *memcpy(void * restrict s1, const void * restrict s2, size_t n)
{
	unsigned char *dst = s1;
	const unsigned char *src = s2;
	for (size_t i = 0; i < n; i++) {
		dst[i] = src[i];
	}
	return s1;
}

void *memset(void *s, int c, size_t n)
{
	unsigned char *p = s;
	for (size_t i = 0; i < n; i++) {
		p[i] = (unsigned char)c;
	}
	return s;
}

int strcmp(const char *s1, const char *s2)
{

	// Remarks:
	// 1) If we made it to the end of both strings (i. e. our pointer points to a
	//    '\0' character), the function will return 0
	// 2) If we didn't make it to the end of both strings, the function will
	//    return the difference of the characters at the first index of
	//    indifference.
	while ((*s1) && (*s1 == *s2)) {
		++s1;
		++s2;
	}
	return (*(unsigned char *)s1 - *(unsigned char *)s2);
}

size_t strlen(const char *s)
{
	size_t len = 0;
	while (*s++) {
		len++;
	}
	return len;
}

char *strtok(char * restrict s1, const char * restrict s2)
{
	static char *tok_tmp = NULL;
	const char *p = s2;

	//new string
	if (s1 != NULL) {
		tok_tmp = s1;
	}
	//old string cont'd
	else {
		if (tok_tmp == NULL) {
			return NULL;
		}
		s1 = tok_tmp;
	}

	//skip leading s2 characters
	while (*p && *s1) {
		if (*s1 == *p) {
			++s1;
			p = s2;
			continue;
		}
		++p;
	}

	//no more to parse
	if (!*s1) {
		return (tok_tmp = NULL);
	}
	//skip non-s2 characters
	tok_tmp = s1;
	while (*tok_tmp) {
		p = s2;
		while (*p) {
			if (*tok_tmp == *p++) {
				*tok_tmp++ = '\0';
				return s1;
			}
		}
		++tok_tmp;
	}

	//end of string
	tok_tmp = NULL;
	return s1;
}

int sprintf(char* str, const char* format, ...){
    va_list valist;
    va_start(valist, format);
    char buffer[512] = {0};
    char ch;
    int index = 0;

    char *temp_str;
    int temp_int;

    while ((ch = *(format++))) {
        if (ch == '%') {
            switch (ch = *format++) {
                case '%':
                    buffer[index++] = '%';
                    break;
                case 's':
                    temp_str = va_arg(valist, char *);
                    for (int i = 0; temp_str[i]; i++) {
                        buffer[index++] = temp_str[i];
                    }
                    break;
                case 'c':
                    buffer[index++] = va_arg(valist, int);
                    break;
                case 'd':
                case'i':
                    temp_int = va_arg(valist, int);
                    temp_str = itoa(temp_int, NULL);
                    for (int i = 0; temp_str[i]; i++) {
                        buffer[index++] = temp_str[i];
                    }
                    break;
            }
        }

        else {
            buffer[index++] = ch;
        }

    }
    for (int i = 0; buffer[i]; i++) {
        str[i] = buffer[i];
    }
    str[index] = '\0';
    return index;
}
