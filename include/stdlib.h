#ifndef MPX_STDLIB_H
#define MPX_STDLIB_H

/**
 @file stdlib.h
 @brief A subset of standard C library functions.
*/

/**
 Convert an ASCII string to an integer
 @param s A NUL-terminated string
 @return The value of the string converted to an integer
*/
int atoi(const char *s);

/**
 * Converts an integer to a ASCII string
 * @param i an integer
 * @return A char* containing the string representation of i
 */
char* itoa(int i,char* dest);

#endif
