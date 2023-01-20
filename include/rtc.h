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
 * @return 0 if failure occurs, 1 otherwise
 */
int setDate();

#endif //WINDOWS_9_RTC_H
