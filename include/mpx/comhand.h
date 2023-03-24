#ifndef MPX_COMHAND_H
#define MPX_COMHAND_H

#include <stddef.h>
#include <pcb.h>
/*
@Name			: init_comhand
@brief			: Initiates and runs the command handler loop
@params			: N/A
@returns		: N/As
*/
void init_comhand(void);

/*
@Name			: comhand_version
@brief			: See comhand.c for more information

@params			: N/A
@returns		: N/A
*/
void comhand_version(void);
/*
@Name			: comhand_help
@brief			: See comhand.c for more information

@params			: N/A
@returns		: N/A
*/
void comhand_help(void);
/*
@Name			: comhand_menu
@brief			: See comhand.c for more information

@params			: N/A
@returns		: N/A
*/
void comhand_menu(void);
/*
@Name			: comhand_shutdown
@brief			: See comhand.c for more information

@params			: N/A
@returns		: N/A
*/
void comhand_shutdown(void);
/*
@Name			: comhand_rtc
@brief			: See comhand.c for more information

@params			: N/A
@returns		: N/A
*/
void comhand_rtc(void);
/*
@Name			: comhand_setTime
@brief			: See comhand.c for more information

@params			: N/A
@returns		: N/A
*/
void comhand_setTime(void);
/*
@Name			: comhand_setDate
@brief			: See comhand.c for more information

@params			: N/A
@returns		: N/A
*/
void comhand_setDate(void);
/*
@Name			: comhand_joeburrow
@brief			: See comhand.c for more information

@params			: N/A
@returns		: N/A
*/
void comhand_joeburrow(void);
/*
@Name			: comhand_pcbCreate
@brief			: See comhand.c for more information

@params			: N/A
@returns		: N/A
*/
void comhand_pcbCreate(void);
/*
@Name			: comhand_pcbDelete
@brief			: See comhand.c for more information

@params			: N/A
@returns		: N/A
*/
void comhand_pcbDelete(void);
/*
@Name			: comhand_pcbBlock
@brief			: See comhand.c for more information

@params			: N/A
@returns		: N/A
*/
void comhand_pcbBlock(void);
/*
@Name			: comhand_pcbUnblock
@brief			: See comhand.c for more information

@params			: N/A
@returns		: N/A
*/
void comhand_pcbUnblock(void);
/*
@Name			: comhand_pcbSuspend
@brief			: See comhand.c for more information

@params			: N/A
@returns		: N/A
*/
void comhand_pcbSuspend(void);
/*
@Name			: comhand_pcbResume
@brief			: See comhand.c for more information

@params			: N/A
@returns		: N/A
*/
void comhand_pcbResume(void);
/*
@Name			: comhand_pcbPriority
@brief			: See comhand.c for more information

@params			: N/A
@returns		: N/A
*/
void comhand_pcbPriority(void);
/*
@Name			: comhand_pcbShow
@brief			: See comhand.c for more information

@params			: entry The number corresponding with the group of PCBs the user wants to show
@returns		: N/A
*/
void comhand_pcbShow(int entry);
/*
@Name			: comhand_pcbShowHelper
@brief			: See comhand.c for more information

@params			: target The PCB to print the data of
@returns		: N/A
*/
void comhand_pcbShowHelper(pcb* target);

void comhand_printPcbList(list* li);
/*
@Name			: comhand_yield()
@brief			: See comhand.c for more information

@param			: N/A
@returns		: N/A
*/
void comhand_yield(void);
/*
@Name			: comhand_load()
@brief			: See comhand.c for more information

@param			: N/A
@returns		: N/A
*/
void comhand_load(void);
/*
@Name			: comhand_alarm()
@brief			: See comhand.c for more information

@param			: N/A
@returns		: N/A
*/
void comhand_alarm(void);
/*
@Name		: comhand_allocateMem
@breif		: will allocate memory

@param		: N/A
@returns	: N/A
*/
void comhand_allocateMem();
/*
@Name		: comhand_freeMem
@breif		: will free memory

@param		: N/A
@returns	: N/A
*/
void comhand_freeMem();
/*
@Name		: comhand_showMem
@brief		: will display a list of allocated or free memory blockes based on user's entry

@param		entry : specifies which 'show' sequence to run
@returns	: N/A
*/
void comhand_showMemory(int entry);
#endif
