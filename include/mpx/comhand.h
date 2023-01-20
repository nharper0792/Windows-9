#ifndef MPX_COMHAND_H
#define MPX_COMHAND_H

#include <stddef.h>

//init_comhand	: initiates and runs the command handler loop
//				: takes no parameters and returns nothing
//				:
//Notes			: See comhand.c for more information.
void init_comhand(void);

//version		: displays the current version of the program to the user
//				: takes no parameters and returns nothing
//				:
//				: See comhand.c for more information
void comhand_version(void);

//help			: displays all commands possible to the user
//				: takes no parameters and returns nothing
//				:
//				: See comhand.c for more information
void comhand_help(void);

//shutdown		: commences shutdown sequence
//				: takes no parameters and returns nothing
//				:
//				: See comhand.c for more information
void comhand_shutdown(void);

//jb			: commences test run of joe burrow sequence
//				: takes no parameters and returns nothing
//				:
//				: See comhand.c for more information
void comhand_jb(void);

//rtc			: displays real-time clock
//				: takes no parameters and returns nothing
//				:
//				: See comhand.c for more information
void comhand_rtc(void);
#endif
