#include <stdlib.h>
#include <ctype.h>

int atoi(const char *s)
{
	int res = 0;
	char sign = ' ';

	while (isspace(*s)) {
		s++;
	}

	if (*s == '-' || *s == '+') {
		sign = *s;
		s++;
	}

	while ('0' <= *s && *s <= '9') {
		res = res * 10 + (*s - '0');
		s++;

	}

	if (sign == '-') {
		res = res * -1;
	}

	return res;
}

char *itoa(int i) {
    int p = 0;
    static char res[10] = {0};
    int isNegative = 0;
    if (i < 0) {
        res[p++] = '-';
        isNegative = 1;
        i = -i;
    }
    for (int j = 10;; j *= 10) {
        if (i < j / 10) {
            res[p] = '\0';
            break;
        } else {
            int remainder = (i % j) / (j / 10);
            if (remainder == 0) {
                res[p++] = '0';
            } else {
                res[p++] = (char)(remainder + 48);
            }
        }
    }
    p--;
    for (int j = isNegative; j < p; j++, p--) {
        char temp = res[j];
        res[j] = res[p];
        res[p] = temp;
    }
    return res;
}