#ifndef MPX_CTYPE_H
#define MPX_CTYPE_H

/**
 @file ctype.h
 @brief A subset of standard C library functions.
*/

/**
 Determine if a character is whitespace.
 @param c Character to check
 @return Non-zero if space, 0 if not space
*/
int isspace(int c);

/**
 *
 * @param c character to check
 * @return Non-zero if digit, 0 if not
 */
int isdigit(int c);

/**
 Convert a char to uppercase
 @param c char to convert case
 @return int new value
*/
int toupper(char c);
#endif
