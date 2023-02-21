#include <mpx/comhand.h>
#include <mpx/serial.h>
#include <memory.h>
#include <sys_req.h>
#include <string.h>
#include <stdlib.h>
#include <rtc.h>
#include <stdio.h>
#include <ctype.h>
#include <pcb.h>

/*
Variable: curr_process
	Use		: tracks which process the handler is currently in
			-	Is counted using a 3-digit base-10 duo of numbers.
			-					e.g 103, 502, 307, etc.
			-	The 100s & 10s place combined (103) represents the current process, ranging [0-7,10-infinity].
			_								   ^^
								e.g process [10]
			-	The 1s place (103) represents the state of the current process.
			-					^
			-					e.g process 10 is in state [3]
			-

*/
int	curr_process = 000;
char yesprompt[] = "YES\0";
char noprompt[] = "NO\0";
//==
void init_comhand(void) {
	comhand_menu();
	for (;;)
	{
		puts("\n> ");
		/*
		Variable: buf
		Use		: buffers packets of data from input devices

		Variable: nread
		Use		: takes a byte of input, to be processed
		*/
		char buf[100] = { 0 };
		int nread = sys_req(READ, COM1, buf, sizeof(buf));
		sys_req(WRITE, COM1, buf, nread);

		//==================================================================
		//	SMALL TEXT GROUPS
		//		This area contains the command prompts that are simple text.
		//==================================================================

		char textversion[]				= "VERSION\0";
		//version
		//process 010
		char textshutdown[]				= "SHUTDOWN\0";
		//shutdown
		//process 020
		char texthelp[]					= "HELP\0";
		//rtc
		//process 040
		char textrtc[]					= "RTC\0";
		//help
		//process 050
		char textsettime[]				= "TIMESET\0";
		//time set
		//process 060
		char textsetdate[]				= "DATESET\0";
		//date set
		//process 070
		char textjoeburrow[]			= "JOE BURROW\0";

		//STAY COMMENTED UNTIL PCB COMMANDS IMPLEMENTED - WILL NOT MAKE IF UNCOMMENTED
		
		char textpcbcreate[]			= "PCB CREATE\0";
		//create PCB
		//process 100
		char textpcbdelete[]			= "PCB DELETE\0";
		//delete PCB
		//process 101
		char textpcbblock[]				= "PCB BLOCK\0";
		//block PCB
		//process 102
		char textpcbshow[]				= "PCB SHOW\0";
		//show PCB
		//process 107
		/*

		char textpcbunblock[]			= "PCB UNBLOCK\0";
		//unblock PCB
		//process 103
		char textpcbsuspend[]			= "PCB SUSPEND\0";
		//suspend PCB
		//process 104
		char textpcbresume[]			= "PCB RESUME\0";
		//resume PCB
		//process 105
		char textpcbpriority[]			= "PCB PRIORITY\0";
		//priority PCB set
		//process 106

		char textpcbshowready[]			= "PCB SHOW READY\0";
		//show PCBs in ready state
		//process 108
		char textpcbshowblocked[]		= "PCB SHOW BLOCKED\0";
		//show PCBs in blocked state
		//process 109
		char textpcbshowall[]			= "PCB SHOW ALL\0";
		//show all PCBs
		//process 1010
		*
		*/

		if ((strcasecmp(texthelp, buf) == 0) ||			atoi(buf) == 1) {
			curr_process = 040;
			comhand_help();
		}
		if ((strcasecmp(textshutdown, buf) == 0) ||		atoi(buf) == 2) {
			curr_process = 020;
			comhand_shutdown();
			if (curr_process == 021)
				return;
		}
		if ((strcasecmp(textversion, buf) == 0) ||		atoi(buf) == 3) {
			curr_process = 010;
			comhand_version();
		}		
		if ((strcasecmp(textrtc, buf) == 0) ||			atoi(buf) == 4) {
			curr_process = 050;
			comhand_rtc();
		}
		if ((strcasecmp(textsettime, buf) == 0) ||		atoi(buf) == 5) {
			curr_process = 060;
			comhand_setTime();
		}
		if ((strcasecmp(textsetdate, buf) == 0) ||		atoi(buf) == 6) {
			curr_process = 070;
			comhand_setDate();
		}
		if ((strcasecmp(textjoeburrow, buf) == 0) ||	atoi(buf) == 7) {
			curr_process = 100;
			comhand_joeburrow();
		}
		if ((strcasecmp(textpcbcreate, buf) == 0) ||	atoi(buf) == 8) {
			curr_process = 100;
			comhand_pcbCreate();
		}
		if ((strcasecmp(textpcbdelete, buf) == 0) ||	atoi(buf) == 9) {
			curr_process = 101;
			comhand_pcbDelete();
		}
		if ((strcasecmp(textpcbshow, buf) == 0) ||		atoi(buf) == 10) {
			curr_process = 107;
			comhand_pcbShow(0);
		}
		if ((strcasecmp(textpcbblock, buf) == 0) ||		atoi(buf) == 13) {
			curr_process = 102;
			comhand_pcbBlock();
		}
		//displays a message to the user stating their prompt wasn't recognized
		//only displays if the user is in the menu process, updates everytime the [ENTER KEY] is read by serial polling.
		else if (curr_process == 000) {
			puts(
				"\n$:Sorry, that command wasn't recognized:"\
				"\n$:Please enter a valid prompt, see help for more information.:"\
				"\n"
			);
		}
		//after user buffer has been read, will update the current process to 0
		curr_process = 000;
	}
}

//==============================================================
//CONTAINS FUNCTIONS USED WITHIN FOREVER LOOP OF COMMAND HANDLER
//==============================================================

//-------------------------------
//BASIC COMMAND HANDLER FUNCTIONS
//-------------------------------

/*
@Name			: comhand_version
@brief			: Will display the current version of the OS. This needs to be manually updated.

@params			: N/A
@returns		: N/A
*/
void comhand_version(void) {
	puts(
		"\n \e[1;95m $:Version:"\
		"\n == Windows 9 JB Edition == "\
		"\nVersion R2.0 \e[0m"\
		"\n"
	);
	comhand_menu();
	return;
}
/*
@Name			: comhand_shutdown
@brief			: Will prompt the user to shutdown the OS, must be cofirmed.

@params			: N/A
@returns		: N/A
*/
void comhand_shutdown(void) {
	puts(
		"\n$:Are you sure you want to shutdown?:"\
		"\n$:	yes"\
		"\n$:	no"\
		"\n"\
		"\n>"
	);

	for (;;) {
		char shutdownconfirmation[10] = { 0 };
		int nread = sys_req(READ, COM1, shutdownconfirmation, sizeof(shutdownconfirmation));
		sys_req(WRITE, COM1, shutdownconfirmation, nread);


		if (strcasecmp(shutdownconfirmation, yesprompt) == 0 )
		{
			sys_req(-1);
			sys_req(EXIT);

			//update process to 021
			//process : commence shutdown
			curr_process = 021;
			return;
		}
		else
		{
			puts(
				"\n$:Aborting shutdown:"\
				"\n$:You will be returned to the main menu:"\
				"\n"
			);
			comhand_menu();
			return;
		}
	}

}
/*
@Name			: comhand_rtc
@brief			: Will display both the month and time of the real time clock. See rtc.c/rtc.h for more information.

@params			: N/A
@return			: N/A
*/
void comhand_rtc(void) {
	puts(
		"\e[1;34m \n$:Real-Time Clock:"
		"\n$:Current Time: "
	);
	char* textrtc_landingclock = getTime();
	sys_req(WRITE, COM1, textrtc_landingclock, sizeof(textrtc_landingclock) + 8);
	puts(
		"\n$:Current Date: "
	);
	char* textrtc_landingtime = getDate();
	sys_req(WRITE, COM1, textrtc_landingtime, sizeof(textrtc_landingtime) + 4);
	puts("\e[0m \n");
	return;
}
/*
@Name			: comhand_setTime
@brief			: Will prompt the user for changing the time of the real time clock. See rtc.c/rtc.h for more information.

@params			: N/A
@return			: N/A
*/
void comhand_setTime(void) {
	for (;;) {
		puts(
			"\e[1;33m \n$:Please enter a new time in the following format:"\
			"\n$:	HH:MM:SS"\
			"\n$:"\
			"\n$:	e.g [Fifteen and a half minutes past noon = 12:15:30]: \e[0m"\
			"\n"\
			"\n>"
		);

		char rtcprompt[100] = { 0 };
		int nread2 = sys_req(READ, COM1, rtcprompt, sizeof(rtcprompt));
		sys_req(WRITE, COM1, rtcprompt, nread2);
		if (
			(rtcprompt[2] == ':') && (rtcprompt[5] == ':')		//string has separators in indexes 2 and 5
			&& ((strlen(rtcprompt)) <= (size_t)8)				//string is less than 8 characters long
			) {
			for (;;) {
				puts(
					"\e[1;33m \n$:Is this the time you'd like to set? \e[0m"\
					"\n$: "
				);
				puts(rtcprompt);
				puts(
					"\e[1;33m \n$:	yes"\
					"\n$:	no \e[0m"\
					"\n"\
					"\n>"
				);

				char timeconfirmation[10] = { 0 };
				int nread2 = sys_req(READ, COM1, timeconfirmation, sizeof(timeconfirmation));
				sys_req(WRITE, COM1, timeconfirmation, nread2);

				if (strcasecmp(timeconfirmation, yesprompt) == 0) {
					break;
				}
				else
				{
					puts(
						"\e[1;33m \n$:Time not set:"\
						"\n$:Returning to menu...: \e[0m"\
						"\n"
					);
					comhand_menu();
					return;
				}
			}
			setTime(rtcprompt);
			puts(
				"\e[1;33m \n$:Time has been changed to: \e[0m"\
				"\n$:"
			);
			puts(getTime());
			puts(
				"\e[1;33m \n$:Returning to Menu...: \e[0m"\
				"\n"
			);
			comhand_menu();
			return;
		}
		else
		{
			puts(
				"\e[0;91m \n$:Invalid format. Please try again: \e[0m"
			);
		}
	}
}
/*
@Name			: comhand_setDate
@brief			: Will prompt the user for changing the date of the real time clock. See rtc.c/rtc.h for more information.

@params			: N/A
@return			: N/A
*/
void comhand_setDate(void) {
	for (;;) {
		puts(
			"\e[1;36m \n$:Please enter a new date in the following format:"\
			"\n$:	MM/DD/YY"\
			"\n$:"\
			"\n$:	e.g [February 18, 2008 = 02/18/08]:"\
			"\n"\
			" \e[0m \n>"
		);

		char rtcprompt[100] = { 0 };
		int nread2 = sys_req(READ, COM1, rtcprompt, sizeof(rtcprompt));
		sys_req(WRITE, COM1, rtcprompt, nread2);
		//test for correct formatting
		if (
			(rtcprompt[2] == '/') && (rtcprompt[5] == '/')		//string has separators in indexes 2 and 5
			&& ((strlen(rtcprompt)) <= (size_t)8)				//string is less than 8 characters long
			) {
			for (;;) {
				puts(
					"\e[1;36m \n$:Is this the date you'd like to set? \e[0m"\
					"\n$:  "
				);
				puts(rtcprompt);
				puts(
					"\e[1;36m \n$:	yes"\
					"\n$:	no"\
					"\n"\
					"\n \e[0m >"
				);

				char dateconfirmation[10] = { 0 };
				int nread2 = sys_req(READ, COM1, dateconfirmation, sizeof(dateconfirmation));
				sys_req(WRITE, COM1, dateconfirmation, nread2);

				if (strcasecmp(dateconfirmation, yesprompt) == 0) {
					break;
				}
				else
				{
					puts(
						"\e[1;36m \n$:Date not set:"\
						"\n$:Returning to menu...: \e[0m"\
						"\n"
					);
					comhand_menu();
					return;
				}
			}
			setDate(rtcprompt);
			puts(
				"\e[1;36m \n$:Date has been changed to: \e[0m"\
				"\n$:"
			);
			puts(getDate());
			puts(
				"\e[1;36m \n$:Returning to Menu...: \e[0m"\
				"\n"
			);
			comhand_menu();
			return;
		}
		else
		{
			puts(
				"\e[0;91m \n$:Invalid format. Please try again: \e[0m"
			);
		}
	}
}
/*
@Name			: comhand_joeburrow
@brief			: Joe Burrow!

@params			: N/A
@return			: N/A
*/
void comhand_joeburrow(void) {
	char burrowbuf[100] = { 0 };
	puts(
		"\n$:This is the System Admin:"\
		"\n$:Giving control to world-renowned football star:"\
		"\n$:Joseph \"Joe\" Thomas Burrow Jr!:"\
	);
	puts(
		"\n"\
		"\n$:Type any prompt to continue..."\
		"\n"\
		"\n> "
	);
	int nread = sys_req(READ, COM1, burrowbuf, sizeof(burrowbuf));
	sys_req(WRITE, COM1, burrowbuf, nread);
	puts(
		"\n$:Do you really not know who Joe Burrow is?:"\
		"\n$:I mean the American football player Joseph \"Joe\" Thomas Burrow, Jr.:"
	);
	puts(
		"\n"\
		"\n$:Type any prompt to continue..."\
		"\n"\
		"\n> "
	);
	nread = sys_req(READ, COM1, burrowbuf, sizeof(burrowbuf));
	sys_req(WRITE, COM1, burrowbuf, nread);
	puts(
		"\n$:He is a quarterback for the Cincinnati Bengals.:"\
		"\n$:He is one of the best quarterbacks the NFL has ever seen in the past several years.:"\
	);
	puts(
		"\n"\
		"\n$:Type any prompt to continue...:"\
		"\n"\
		"\n> "
	);
	nread = sys_req(READ, COM1, burrowbuf, sizeof(burrowbuf));
	sys_req(WRITE, COM1, burrowbuf, nread);
	puts(
		"\n$:And if my memory holds up correctly,:" \
		"\n$:he recently won the NFL award as the Most Valuable Player.:" \
		"\n$:Still no? Well, here he is, Joe Burrow!:"
	);
	puts(
		"\n"\
		"\n$:Type any prompt to continue...:"\
		"\n"\
		"\n> "
	);
	nread = sys_req(READ, COM1, burrowbuf, sizeof(burrowbuf));
	sys_req(WRITE, COM1, burrowbuf, nread);

	puts(
		"\n(%): It's me, Joe Burrow! I'm a real person!"\
		"\n(%): You gotta help me save the superbowl from the Chiefs!"\
		"\n(%): Will you help me?"\
		"\n"\
		"\n> "
	);
	nread = sys_req(READ, COM1, burrowbuf, sizeof(burrowbuf));
	sys_req(WRITE, COM1, burrowbuf, nread);
	if (strcasecmp(burrowbuf, yesprompt) == 0) {
		puts(
			"\n(%): Awesome! Glad you could be a help, my little buddy."\
			"\n(%): Now let's get you up to speed."\
			"\n(%): The window is my whole career."\
			"\n(%): ..."\
			"\n(%): Will you be my whole career?"\
			"\n"\
			"\n> "
		);
	}
	else {
		puts(
			"\n\e[1;95mJoe Burrow looks at you with a sad look on his face."\
			"\n\e[1;95mHe lets out a sigh, mumbling \"Who dey, who dey\" under his breath."\
			"\n\e[1;95mHe leaves, never to be seen again."
		);
		puts(
			"\n\e[1;00m$:Returning to menu...:"
		);
		comhand_menu();
		return;
	}
	nread = sys_req(READ, COM1, burrowbuf, sizeof(burrowbuf));
	sys_req(WRITE, COM1, burrowbuf, nread);
	if (strcasecmp(burrowbuf, yesprompt) == 0) {
		puts(
			"\n(%): Really? No one has ever...played football..."\
			"\n(%): with me..."\
			"\n(%): You'd really play...with me?"\
			"\n"\
			"\n> "
		);
	}
	else {
		puts(
			"\n\e[1;95mJoe Burrow liked this conversation until now."\
			"\n\e[1;95mHe lets out a sigh, mumbling \"The Bengals are going all the way this year\" under his breath."\
			"\n\e[1;95mHe uses his winged feet to fly away, taking his true place amongst the clouds."
		);
		puts(
			"\n\e[1;00m$:Returning to menu...:"
		);
		comhand_menu();
		return;
	}
	nread = sys_req(READ, COM1, burrowbuf, sizeof(burrowbuf));
	sys_req(WRITE, COM1, burrowbuf, nread);
	puts(
		"\n\e[1;95mJoe Burrow looks at you with deep, tear-filled eyes."\
		"\n\e[1;95mHe doesn't know how to process what you just said."\
	);
	puts(
		"\n\e[1;00m$:Returning to menu...:"
	);
	comhand_menu();
	return;
}

//-------------
//PCB FUNCTIONS
//-------------

/*
@Name			: comhand_pcbCreate
@brief			: Will prompt the user to enter formatted parameters for PCB creation. Will then ask if they want to confirm changes, adding the PCB to the appropriate list.

@params			: N/A
@returns		: N/A
*/
void comhand_pcbCreate(void) {
	char pcbbuf[100] = { 0 };

	for (;;) {
		//prompt for user, yes or no question
		puts(
			"\n$:Would you like to create a new PCB?:"\
			"\n$:	yes"\
			"\n$:	no"\
			"\n> "
		);
		//read buffer||give user command
		int nread = sys_req(READ, COM1, pcbbuf, sizeof(pcbbuf));
		sys_req(WRITE, COM1, pcbbuf, nread);
		//yes case
		if (strcasecmp(pcbbuf, yesprompt) == 0) {
			puts(
				"\n$:Entering PCB creation...:"\
				"\n"
			);
			break;
		}
		//no case
		else if (strcasecmp(pcbbuf, noprompt) == 0) {
			puts(
				"\n$:PCB creation cancelled."\
				"\n$:Returning to menu...:"\
				"\n"
			);
			comhand_menu();
			return;
		}
		//anything else case
		else {
			puts(
				"\n$:Invalid format. Please try again.:"\
				"\n"
			);
		}
	}
	//pcb name
	puts(
		"\n$:Please enter the desired name of your new PCB:"\
		"\n> "
	);
	//read buffer||give user command
	int nread = sys_req(READ, COM1, pcbbuf, sizeof(pcbbuf));
	sys_req(WRITE, COM1, pcbbuf, nread);
	//capture pcb name
	char* pcbName = sys_alloc_mem(sizeof(pcbbuf));
	strcpy(pcbName, pcbbuf);
	/*
	Variable Name : pcbClass
	Variable Desc : Captures the class of the user prompt PCB.
					This ranges from 0-1, 0 being a user process and 1 being a system process
	*/
	int pcbClass;
	//pcb class
	for (;;) {
		puts(
			"\n$:Please enter the desired class of your new PCB:"\
			"\n$:	user"\
			"\n$:	system"\
			"\n> "
		);
		//read buffer||give user command
		nread = sys_req(READ, COM1, pcbbuf, sizeof(pcbbuf));
		sys_req(WRITE, COM1, pcbbuf, nread);
		//capture pcb class
		char pcbuserprompt[] = "USER\0";
		char pcbsystemprompt[] = "SYSTEM\0";
		//user case
		if (strcasecmp(pcbuserprompt, pcbbuf) == 0) {
			puts(
				"\n$:Your new PCB has been given the [user] class"\
				"\n"
			);
			pcbClass = 0;
			break;
		}
		//system case
		else if (strcasecmp(pcbsystemprompt, pcbbuf) == 0) {
			puts(
				"\n$:Your new PCB has been given the [system] class"\
				"\n"
			);
			pcbClass = 1;
			break;
		}
		//all other input
		else {
			puts(
				"\n$:Invalid format. Please try again.:"\
				"\n"
			);
		}
	}
	/*
	Variable Name : pcbPriority
	Variable Desc : Captures the priority of the user prompt PCB.
					This ranges from 0-9.
	*/
	int pcbPriority;
	//pcb priority
	for (;;) {
		puts(
			"\n$:Please enter the desired priority of your new PCB:"\
			"\n$:This number must range from [0-9]."\
			"\n> "
		);
		//read buffer||give user command
		nread = sys_req(READ, COM1, pcbbuf, sizeof(pcbbuf));
		sys_req(WRITE, COM1, pcbbuf, nread);
		//capture pcb name
		if (strlen(pcbbuf) == 1 &&
			(isdigit(pcbbuf[0]) >= 0) &&
			(isdigit(pcbbuf[0]) <= 9)) {
			pcbPriority = atoi(pcbbuf);
			puts("\n$:PCB Priority set to ");
			puts(pcbbuf);
			puts(":");
			break;
		}
		else {
			puts(
				"\n$:Invalid format. Please try again:"\
				"\n"
			);
		}
	}
	//print name
	printf(
		"\n"\
		"\n$:Creating new PCB with these parameters."\
		"\n$:Name: %s",
		pcbName
	);
	
	//print class
	puts("\n$:Class: ");
	if (pcbClass == 0)
		puts("USER");
	else
		puts("SYSTEM");
	//print priority
	printf(
		"\n$:Priority: %i",
		pcbPriority
	);
	//confirmation + message
	for (;;) {
		puts(
			"\n"\
			"\n$:Confirm creation of PCB with these settings?"\
			"\n$:	yes"\
			"\n$:	no"\
			"\n> "
		);
		nread = sys_req(READ, COM1, pcbbuf, sizeof(pcbbuf));
		sys_req(WRITE, COM1, pcbbuf, nread);
		//yes case
		if (strcasecmp(yesprompt, pcbbuf) == 0) {
			//create pcb
			pcb* dummy = pcb_setup(
				pcbName, 
				pcbClass, 
				pcbPriority
			);
			//insert pcb
			pcb_insert(dummy);
			//confirmation statement
			printf(
				"\n$:Creation of PCB %s was successful:"\
				"\n",
				dummy->name
			);
			break;
		}
		//no case
		else if (strcasecmp(noprompt, pcbbuf) == 0) {
			puts("\n$:PCB creation cancelled:");
			break;
		}
		//all other responses
		else {
			puts("\n$:Invalid format. Please try again:");
		}
	}
	//return statement
	puts(
		"\n$:Returning to menu...:"\
		"\n"
	);
	//free(pcbName);
	comhand_menu();
	return;
}
/*
@Name			: comhand_pcbDelete
@brief			: This command will prompt the user to input the name of a PCB that they want to delete. 
				  If found, will delete that PCB. If not, returns to menu.

@params			: N/A
@returns		: N/A
*/
void comhand_pcbDelete(void) {
	char pcbbuf[100] = { 0 };
		//pcb name
		puts(
			"\n$:Please enter the name of the PCB you would like to delete:"\
			"\n> "
		);
		//read buffer||give user command
		int nread = sys_req(READ, COM1, pcbbuf, sizeof(pcbbuf));
		sys_req(WRITE, COM1, pcbbuf, nread);
		//capture pcb name
		char* pcbName = sys_alloc_mem(sizeof(pcbbuf));
		strcpy(pcbName, pcbbuf);
		//create dummy pcb
		pcb* dummy = pcb_find(pcbName);
		//if found, delete
		if (dummy != NULL) {
			//remove pcb from pcb lists
			pcb_remove(dummy);
			//free pcb
			pcb_free(dummy);
			puts(
				"\n$:PCB deleted:"\
				"\n$:Returning to menu...:"\
				"\n"
			);
			return;
		}
		else {
			printf(
				"\n$:PCB %s not found in PCB list(s):"\
				"\n",
				pcbName
			);
		}
		puts(
			"\n$:Returning to menu...:"\
			"\n"
		);
		comhand_menu();
		return;
	
}
/*
@Name			: comhand_pcbBlock
@brief			: prompts the user to input the name of a PCB they would like to switch to the [BLOCKED] execution state.

@params			: N/A
@returns		: N/A
*/
void comhand_pcbBlock(void) {
	char pcbbuf[100] = { 0 };
	puts(
		"\n$:Please enter the name of the PCB you would like to switch to the [BLOCKED] execution state:"\
		"\n> "
	);
	//read buffer||give user command
	int nread = sys_req(READ, COM1, pcbbuf, sizeof(pcbbuf));
	sys_req(WRITE, COM1, pcbbuf, nread);
	//capture input
	const char* pcbName = pcbbuf;
	
	pcb* dummy = pcb_find(pcbName);

	if (dummy != NULL) {
		dummy->executionState = BLOCKED;
		printf(
			"\n$:PCB %s has been given the [BLOCKED] execution state",
			pcbName
		);
	}
	else {
		puts(
			"\n$:PCB not found."
		);
	}
	//return
	comhand_menu();
	return;
}
/*
@Name			:
@brief			:

@params			: N/A
@returns		: N/A
*/
void comhand_pcbUnblock(void) {
	return;
}
/*
@Name			:
@brief			:

@params			: N/A
@returns		: N/A
*/
void comhand_pcbSuspend(void) {
	return;
}
/*
@Name			:
@brief			:

@params			: N/A
@returns		: N/A
*/
void comhand_pcbResume(void) {
	return;
}
/*
@Name			: comhand_pcbPriority
@brief			: Prompts the user to enter the name of the PCB they want to change the priority of. 
				  Then changes the priority of that PCB.

@params			: N/A
@returns		: N/A
*/
void comhand_pcbPriority(void) {
	char pcbbuf[100] = { 0 };
	//pcb name
	puts(
		"\n$:Please enter the name of the PCB you would like to change priority:"\
		"\n> "
	);
	//read buffer||give user command
	int nread = sys_req(READ, COM1, pcbbuf, sizeof(pcbbuf));
	sys_req(WRITE, COM1, pcbbuf, nread);
	//capture pcb name
	char* pcbName = sys_alloc_mem(sizeof(pcbbuf));
	strcpy(pcbName, pcbbuf);

	//if found, prompt for new priority
	if (pcb_find(pcbName) != NULL) {
		//assign dummyPCB for modification of user-specified PCB
		struct pcb* dummyPCB = pcb_find(pcbName);
		printf(
			"\n$:PCB %s currently has priority %i:",
			pcbName, 
			dummyPCB->priority
		);

		int pcbPriority;
		//pcb priority capture
		for (;;) {
			printf(
				"\n$:Please enter the new desired priority of PCB %s:"\
				"\n$:This number must range from [0-9]."\
				"\n> ",
				pcbName
			);
			//read buffer||give user command
			nread = sys_req(READ, COM1, pcbbuf, sizeof(pcbbuf));
			sys_req(WRITE, COM1, pcbbuf, nread);
			//capture pcb name
			if (strlen(pcbbuf) == 1 &&
				(isdigit(pcbbuf[0]) >= 0) &&
				(isdigit(pcbbuf[0]) <= 9)) {
				pcbPriority = atoi(pcbbuf);
				printf(
					"\n$:PCB %s's priority set to %s :",
					pcbName, 
					pcbbuf
				);
				break;
			}
			//fail case
			else {
				puts(
					"\n$:Invalid format. Please try again:"\
					"\n"
				);
			}
		}
		//update PCB's priority
		dummyPCB->priority = pcbPriority;
		//free dummy
		pcb_free(dummyPCB);
	}
	else {
		printf(
			"\n$:PCB %s not found in PCB list(s):"\
			"\n",
			pcbName
		);
	}

	//free(pcbName);
	comhand_menu();
	return;

}
/*
@Name			: comhand_pcbShow
@brief			: Will show existing PCBs to the user. 
				  What type of show that is done is specified in main command handler by the user.

@param			entry : specifies which 'show' sequence to run
@returns		: N/A
*/
void comhand_pcbShow(int entry) {
	//char pcbbuf[100] = { 0 };
	//show specific PCB, prompt user
	if (entry == 0) {
		char pcbbuf[100] = { 0 };
		puts(
			"\n$:Please enter the name of the PCB you would like to show:"\
			"\n> "
		);
		//read buffer||give user command
		int nread = sys_req(READ, COM1, pcbbuf, sizeof(pcbbuf));
		sys_req(WRITE, COM1, pcbbuf, nread);
		//capture input
		const char* pcbName = pcbbuf;
		//create PCB from user input
		pcb* dummy = pcb_find(pcbName);
		if (dummy != NULL) {
			printf(
				"\n"\
				"\n$:Your PCB's parameters are shown below:"\
				"\n	Name				: %s "\
				"\n	Priority			: %i "\
				"\n	Class Level			: [FINISH ENUM CONVERSION] "\
				"\n	Execution State		: [FINISH ENUM CONVERSION] "\
				"\n	Dispatching State	: [FINISH ENUM CONVERSION] "\
				"\n"
			);
		}
		else {
			printf(
				"\n$:PCB %s not found:"\
				"\n$:Returning to menu...:"\
				"\n"
			);
		}
		comhand_menu();
		return;
	}
	//show ready PCBs
	if (entry == 1) {

	}
	//show blocked PCBs
	if (entry == 2) {

	}
	//show all PCBs
	if (entry == 3) {
		for (;;) {
			
		}
	}
	(void)entry;
	return;
}
//========================================================================
//  COMHAND HELP SECTION
// 
//	CONTAINS THE VARIOUS HELP COMMANDS AND DESCRIPTIONS ON HOW TO USE THEM
// 
//========================================================================

/*
@Name			: comhand_help/comhand_menu
@brief			: Will display the help results. User can enter additional number to indicate which subsection they would like to see more details on

@params			: N/A
@return			: N/A
*/
void comhand_help(void) {
	puts(
		"\n\e[1;92m $:Commands:"\
		"\n$:"\
		"\n$:	1) help"\
		"\n$:		Displays all available commands to the user."\
		"\n$:	2) shutdown"\
		"\n$:		Prompts the user for the shutdown procedure."\
		"\n$:	3) version"\
		"\n$:		Displays the Windows-9 current version."\
		"\n$:	4) rtc"\
		"\n$:		Displays the realtime clock, and prompts the user for clock changes."\
		"\n$:	5) timeset"\
		"\n$:		Prompts the user to change the time of the real-time clock."\
		"\n$:	6) dateset"\
		"\n$:		Prompts the user to change the date of the real-time clock."\
		"\n$:	7) joe burrow"\
		"\n$:		Gives you a real-life chat with superstar Joe Burrow!"\
		"\n$:	8) pcb create"\
		"\n$:		Enters PCB creation mode, where parameters are inputted to create a new PCB."\
		"\n$:	9) pcb delete"\
		"\n$:		Enters PCB deletion mode, where parameters are inputted to delete an existing PCB."\
		"\n$:	10) pcb show"\
		"\n$:		Will show the specific PCB that the user specifies."\
		"\n$:	11)"\
		"\n$:"\
		"\n$:	12)"\
		"\n$:"\
		"\n$:	13) pcb block"\
		"\n$:		Switches the state of a PCB to [BLOCKED] execution state."\
		"\n$:"\
		"\n$:  \n \e[0m"
	);
	return;
}
void comhand_menu(void) {
	puts(
		"\n$:Commands:"\
		"\n$:"\
		"\n$:	1) help"\
		"\n$:	2) shutdown"\
		"\n$:	3) version"\
		"\n$:	4) rtc"\
		"\n$:	5) timeset"\
		"\n$:	6) dateset"\
		"\n$:	7) joe burrow"\
		"\n$:	8) pcb create"\
		"\n$:	9) pcb delete"\
		"\n$:	10) pcb show"\
		"\n$:	11) "\
		"\n$:	12) "\
		"\n$:	13) pcb block "\
		"\n$:"\
		"\n$:See help command for more information.: "\
		"\n"
	);
	return;
}