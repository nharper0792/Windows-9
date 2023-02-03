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

//menu			: displays all raw commands possible to the user
//				: takes no parameters and returns nothing
//				:
//				: See comhand.c for more information
void comhand_menu(void);
//shutdown		: commences shutdown sequence
//				: takes no parameters and returns nothing
//				:
//				: See comhand.c for more information
void comhand_shutdown(void);
//rtc			: displays real-time clock
//				: takes no parameters and returns nothing
//				:
//				: See comhand.c for more information
void comhand_rtc(void);
//setTime		: sets the realtime clock's time
//				: takes no parameters and returns nothing
//				:
//				: See comhand.c for more information
void comhand_setTime(void);
//setDate		: sets the realtime clock's date
//				: takes no parameters and returns nothing
//				:
//				: See comhand.c for more information
void comhand_setDate(void);

void comhand_joeburrow(void);
#endif
