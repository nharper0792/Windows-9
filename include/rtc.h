//
// Created by Nate Harper on 1/17/2023.
//

#ifndef WINDOWS_9_RTC_H
#define WINDOWS_9_RTC_H

/**
 *
 * @brief Returns the current date in string format
 * @return char* with the current date
 */
char* getDate();
/**
 * @brief writes the date to the RTC
 * @param char* newDate - a string containing the new date in mm/dd/yy format
 */
void setDate(char* newDate);

/**
 * @brief returns the current time in UTC
 * @return
 */
char* getTime();

/**
 * @brief writes the time to the RTC
 * @param newDate (char*) - a string containing the new date in the format HH:MM:SS format
 */
void setTime(char* newDate);
/**
 * @brief converts a binary number to decimal
 * @param int src - the integer to convert
 */
int bcdToDec(int src);
/**
 * @brief converts a decimal number to binary
 * @param int src - the integer to convert
 */
int decToBcd(int src);
#endif //WINDOWS_9_RTC_H
