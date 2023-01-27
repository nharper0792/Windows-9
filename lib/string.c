#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <ctype.h>
#include <memory.h>
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

char *pad(char *s, int padding, char type) {
    int len = strlen(s);
    if (len > padding) {
        return s;
    } else {
        char *ret = (char *)sys_alloc_mem(padding);
        for (int i = 0; i < padding - len; i++) {
            ret[i] = type;
        }
        for (int i = padding - len, j = 0; i < padding; j++, i++) {
            ret[i] = s[j];
        }
        return ret;
    }

}
char *formatCore(const char *format, va_list valist) {
    char *buffer = (char *)sys_alloc_mem(512);
    char ch;
    int index = 0;

    int padding = 0;
    int pad_with_zeros = 0;
    char *temp_str;
    int temp_int;

    while ((ch = *(format++))) {
        if (ch == '%') {
            start:
            switch (ch = *format++) {
                case '%':
                    buffer[index++] = '%';
                    break;
                case 's':
                    temp_str = va_arg(valist, char *);
                    if (padding) {
                        temp_str = pad(temp_str, padding, ' ');
                    }
                    for (int i = 0; temp_str[i]; i++) {
                        buffer[index++] = temp_str[i];
                    }
                    break;
                case 'c':
                    buffer[index++] = va_arg(valist, int);
                    break;
                case 'd':
                case 'i':
                    temp_int = va_arg(valist, int);
                    temp_str = itoa(temp_int, NULL);
                    if (padding) {
                        temp_str = pad(temp_str, padding, pad_with_zeros ? '0' : ' ');
                    }
                    for (int i = 0; temp_str[i]; i++) {
                        buffer[index++] = temp_str[i];
                    }
                    break;
                default:
                    if (ch == '.') {
                        pad_with_zeros = 1;
                        ch = *format++;
                    }
                    if (isdigit(ch)) {
                        char temp[4] = {0};
                        temp[0] = ch;
                        int ptr = 1;
                        while (isdigit(*(format))) {
                            ch = *format++;
                            temp[ptr++] = ch;
                        }
                        padding = atoi(temp);
                        goto start;
                    }
                    break;
            }

            padding = 0;
            pad_with_zeros = 0;
        } else {
            buffer[index++] = ch;
        }
    }

    buffer[index] = '\0';
    return buffer;
}
int sprintf(char* str, const char* format, ...) {
    va_list valist;
    va_start(valist, format);
    char* buffer = formatCore(format,valist);
    va_end(valist);
    for (int i = 0; buffer[i]; i++) {
        str[i] = buffer[i];
    }
    return strlen(str);
}
