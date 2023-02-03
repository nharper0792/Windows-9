#include <mpx/comhand.h>
#include <mpx/serial.h>
#include <memory.h>
#include <sys_req.h>
#include <string.h>
#include <rtc.h>
#include <stdio.h>
#include <ctype.h>

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

		char textversion[] = "VERSION\0";
		//version
		//process 010
		char textshutdown[] = "SHUTDOWN\0";
		//shutdown
		//process 020
		char texthelp[] = "HELP\0";
		//rtc
		//process 040
		char textrtc[] = "RTC\0";
		//help
		//process 050
		char textsettime[] = "TIMESET\0";
		//time set
		//process 060
		char textsetdate[] = "DATESET\0";
		//date set
		//process 070
		if ((strcmp(textversion, toupper(buf)) == 0)) {
			curr_process = 010;
			comhand_version();
		}
		if ((strcmp(textshutdown, toupper(buf)) == 0)) {
			curr_process = 020;
			comhand_shutdown();
			if (curr_process == 021)
				return;
		}
		if ((strcmp(texthelp, toupper(buf)) == 0)) {
			curr_process = 040;
			comhand_help();
		}
		if ((strcmp(textrtc, toupper(buf)) == 0)) {
			curr_process = 050;
			comhand_rtc();
		}
		if ((strcmp(textsettime, toupper(buf)) == 0)) {
			curr_process = 060;
			comhand_setTime();
		}
		if ((strcmp(textsetdate, toupper(buf)) == 0)) {
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
		"\n \e[1;95m $:Version:"\
		"\n == Windows 9 JB Edition == "\
		"\nVersion R1.0 \e[0m"\
		"\n"
	);
	comhand_menu();
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
		"\n"\
		"\n>"
	);

	for (;;) {
		char shutdownconfirmation[10] = { 0 };
		int nread = sys_req(READ, COM1, shutdownconfirmation, sizeof(shutdownconfirmation));
		sys_req(WRITE, COM1, shutdownconfirmation, nread);


		if (strcmp(toupper(shutdownconfirmation), yesprompt) == 0)
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
Function Name	: comhand_rtc
Function Desc	: Will display both the month and time of the real time clock. See rtc.c/rtc.h for more information.

@params			: N/A
@return			: N/A
*/
void comhand_rtc(void) {
	puts(
		"\e[1;34m \n$:Real-Time Clock:"
		"\n$:Current Time: "
	);
	char* textrtc_landingclock = getTime();
	sys_req(WRITE, COM1, textrtc_landingclock, sizeof(textrtc_landingclock) + 5);
	puts(
		"\n$:Current Date:"
	);
	char* textrtc_landingtime = getDate();
	sys_req(WRITE, COM1, textrtc_landingtime, sizeof(textrtc_landingtime) + 4);
	puts("\e[0m \n");
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

				if (strcmp(toupper(timeconfirmation), yesprompt) == 0) {
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
Function Name	: comhand_setDate
Function Desc	: Will prompt the user for changing the date of the real time clock. See rtc.c/rtc.h for more information.

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

				if (strcmp(toupper(dateconfirmation), yesprompt) == 0) {
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

//========================================================================
//  COMHAND HELP SECTION
// 
//	CONTAINS THE VARIOUS HELP COMMANDS AND DESCRIPTIONS ON HOW TO USE THEM
// 
//========================================================================

/*
Function Name	: comhand_help/comhand_menu
Function Desc	: Will display the help results. User can enter additional number to indicate which subsection they would like to see more details on

@params			: N/A
@return			: N/A
*/
void comhand_help(void) {
	puts(
		"\n \e[1;92m $:Commands:"\
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
		"\n$:		Prompts the user to change the date of the real-time clock. "\
		"\n$:  \n \e[0m"
	);
	return;
}
void comhand_menu(void) {
	puts(
		"\n $:Commands:"\
		"\n$:"\
		"\n$:	0) help"\
		"\n$:	1) shutdown"\
		"\n$:	2) version"\
		"\n$:	3) rtc"\
		"\n$:	4) timeset"\
		"\n$:	5) dateset"\
		"\n$:"\
		"\n$:See help command for more information.: "\
		"\n"
	);
	return;
}