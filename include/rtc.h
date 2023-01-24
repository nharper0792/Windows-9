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

int setTime();
#endif //WINDOWS_9_RTC_H
