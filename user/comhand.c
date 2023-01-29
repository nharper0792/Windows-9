#include <mpx/comhand.h>
#include <mpx/serial.h>
#include <memory.h>
#include <sys_req.h>
#include <string.h>
#include <rtc.h>
#include <stdio.h>

/*
Variable: curr_process
	Use		: tracks which process the handler is currently in
			-	Is counted using a 3-digit base-10 duo of numbers.
			-					e.g 103, 502, 307, etc.
			-	The 100s & 10s place combined (103) represents the current process.
			_								   ^^
								e.g process [10]
			-	The 1s place (103) represents the state of the current process.
			-					^
			-					e.g process 10 is in state [3]
			-

*/
int	curr_process = 000;
char yesprompt[] = "yes\0";
char noprompt[] = "no\0";
//==
void init_comhand(void) {
	/*
	Variable: dummy
	Use		: mutates pointer into string needed for current output
	Purpose	: This variable provides a way to save space by using a single pointer to output the various texts used throughout the init_comhand() function
	*/
	comhand_help();
	for (;;)
	{
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

		char textversion[] = "version\0";
		//version
		//process 010
		char textshutdown[] = "shutdown\0";
		//shutdown
		//process 020
		char texthelp[] = "help\0";
		//rtc
		//process 040
		char textrtc[] = "rtc\0";
		//help
		//process 050
		char textsettime[] = "timeset\0";
		//time set
		//process 060
		char textsetdate[] = "dateset\0";
		//date set
		//process 070
		if ((strcmp(textversion, buf) == 0)) {
			curr_process = 010;
			comhand_version();
		}
		if ((strcmp(textshutdown, buf) == 0)) {
			curr_process = 020;
			comhand_shutdown();
			if (curr_process == 021)
				return;
		}
		if ((strcmp(texthelp, buf) == 0)) {
			curr_process = 040;
			comhand_help();
		}
		if ((strcmp(textrtc, buf) == 0)) {
			curr_process = 050;
			comhand_rtc();
		}
		if ((strcmp(textsettime, buf) == 0)) {
			curr_process = 060;
			comhand_setTime();
		}
		if ((strcmp(textsetdate, buf) == 0)) {
			curr_process = 070;
			comhand_setDate();
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

/*
Function Name	: comhand_version
Function Desc	: Will display the current version of the OS. This needs to be manually updated.

@params			: N/A
@return			: N/A
*/
void comhand_version(void) {
	puts(
		"\n$:Version:"\
		"\n == Windows 9 JB Edition == "\
		"\nVersion R1.0"\
		"\n"
	);
	return;
}
/*
Function Name	: comhand_shutdown
Function Desc	: Will prompt the user to shutdown the OS, must be cofirmed.

@params			: N/A
@return			: N/A
*/
void comhand_shutdown(void) {
	puts(
		"\n$:Are you sure you want to shutdown?:"\
		"\n$:	yes"\
		"\n$:	no"\
		"\n"
	);

	for (;;) {
		char shutdownconfirmation[10] = { 0 };
		int nread = sys_req(READ, COM1, shutdownconfirmation, sizeof(shutdownconfirmation));
		sys_req(WRITE, COM1, shutdownconfirmation, nread);


		if (strcmp(shutdownconfirmation, yesprompt) == 0)
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
			return;
		}
	}

}
/*
Function Name	: comhand_rtc
Function Desc	: Will display both the month and time of the real time clock. See rtc.c/rtc.h for more information.

@params			: N/A
@return			: N/A
*/
void comhand_rtc(void) {
	puts(
		"\n$:Real-Time Clock:"
		"\n$:Current Time: "
	);
	char* textrtc_landingclock = getTime();
	sys_req(WRITE, COM1, textrtc_landingclock, sizeof(textrtc_landingclock) + 5);
	puts(
		"\n$:Current Date: "
	);
	char* textrtc_landingtime = getDate();
	sys_req(WRITE, COM1, textrtc_landingtime, sizeof(textrtc_landingtime) + 4);
	puts("\n");
	return;
}
/*
Function Name	: comhand_setTime
Function Desc	: Will prompt the user for changing the time of the real time clock. See rtc.c/rtc.h for more information.

@params			: N/A
@return			: N/A
*/
void comhand_setTime(void) {
	for (;;) {
		puts(
			"\n$:Please enter a new time in the following format:"\
			"\n$:	HH:MM:SS"\
			"\n$:"\
			"\n$:	e.g [Fifteen and a half minutes past noon = 12:15:30]:"\
			"\n"
		);

		char rtcprompt[100] = { 0 };
		int nread2 = sys_req(READ, COM1, rtcprompt, sizeof(rtcprompt));
		sys_req(WRITE, COM1, rtcprompt, nread2);
		setTime(rtcprompt);
		puts(
			"\n$:Time has been changed to:"\
			"\n$:"
		);
		puts(getTime());
		puts("\n");
		puts(
			"\n$:Returning to Menu...:"\
			"\n"
		);
		return;
	}
}
/*
Function Name	: comhand_setDate
Function Desc	: Will prompt the user for changing the date of the real time clock. See rtc.c/rtc.h for more information.

@params			: N/A
@return			: N/A
*/
void comhand_setDate(void) {
	for (;;) {
		puts(
			"\n$:Please enter a new date in the following format:"\
			"\n$:	MM/DD/YYYY:"\
			"\n$:"\
			"\n$:	e.g [February 18, 2008 = 02/18/2008]:"\
			"\n"
		);

		char rtcprompt[100] = { 0 };
		int nread2 = sys_req(READ, COM1, rtcprompt, sizeof(rtcprompt));
		sys_req(WRITE, COM1, rtcprompt, nread2);
		setDate(rtcprompt);
		puts(
			"\n$:Date has been changed to:"\
			"\n$:"
		);
		puts(getDate());
		puts("\n");
		puts(
			"\n$:Returning to Menu...:"\
			"\n"
		);
		return;
	}
}

//========================================
//COMHAND HELP SECTION
// 
//	CONTAINS THE VARIOUS HELP COMMANDS AND DESCRIPTIONS ON HOW TO USE THEM
// 
//========================================

/*
Function Name	:
Function Desc	: Will display the help results. User can enter additional number to indicate which subsection they would like to see more details on

@params			: N/A
@return			: N/A
*/
void comhand_help(void) {
	puts(
		"\n$:Commands:"\
		"\n$:"\
		"\n$:	0) help"\
		"\n$:		Displays all available commands to the user."\
		"\n$:	1) shutdown"\
		"\n$:		Prompts the user for the shutdown procedure."\
		"\n$:	2) version"\
		"\n$:		Displays the Windows-9 current version."\
		"\n$:	3) rtc"\
		"\n$:		Displays the realtime clock, and prompts the user for clock changes."\
		"\n$:	4) timeset"\
		"\n$:		Prompts the user to change the time of the real-time clock."\
		"\n$:	5) dateset"\
		"\n$:		Prompts the user to change the date of the real-time clock."\
		"\n$:\n"
	);
	return;
}