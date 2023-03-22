//
// Created by Owner on 3/18/2023.
//
#include <linked-list.h>
#ifndef WINDOWS_9_COMMAND_HISTORY_H
#define WINDOWS_9_COMMAND_HISTORY_H
#define NULL ((void*)0)
void addToHistory(const char* command);
char* getFromHistory();
void addToCycled(const char* command);
char* getFromCycled();
void resetHistory();
#endif //WINDOWS_9_COMMAND_HISTORY_H
