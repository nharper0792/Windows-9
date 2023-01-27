/*
TODO COMHAND

WORK ON FULL WORDS (polling)
WORK ON EXTRA COMMANDS
CONFIRM ERROR CASES
CONFIRM EDGE CASES

CONVERT ALL TO PUTS**[done, need to push]
*/

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

//==
void init_comhand(void) {
	/*
	Variable: dummy
	Use		: mutates pointer into string needed for current output
	Purpose	: This variable provides a way to save space by using a single pointer to output the various texts used throughout the init_comhand() function
	*/
	puts("$:You have arrived!:\n");

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

		//==
		//	SMALL TEXT GROUPS
		//		This area contains the command prompts that are simple text.
		//==

		char textversion[] = "version\0";
		//version
		//process 010
		if ((strcmp(textversion, buf) == 0) && nread == 10) {
			curr_process = 010;
			comhand_version();
		}

		char textshutdown[] = "shutdown\0";
		//shutdown
		//process 020
		if ((strcmp(textshutdown, buf) == 0) && (nread == 10)) {
			curr_process = 020;
			comhand_shutdown();

			if (curr_process == 021)
				return;
			else
				curr_process = 000;
		}

		char texthelp[] = "help";
		//help
		//process 040
		if ((strcmp(texthelp, buf) == 0) && nread == 10) {
			curr_process = 040;
			comhand_help();
		}

		char textrtc[] = "rtc\0";
		//help
		//process 050
		if ((strcmp(textrtc, buf) == 0) && nread == 10) {
			curr_process = 050;
			comhand_rtc();
		}

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
		"\nVersion R1.0\n"
	);
	curr_process = 0;
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
		"\n$:	1) yes"\
		"\n$:	1) no"
	);

	for (;;) {
		char shutdownconfirmation[25] = { 0 };
		int nread = sys_req(READ, COM1, shutdownconfirmation, sizeof(shutdownconfirmation));
		sys_req(WRITE, COM1, shutdownconfirmation, nread);

		char yesprompt[] = "yes\0";
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
				"\n$:You will be returned to the main menu:"
			);

			curr_process = 000;
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
	char* textrtc_landingclock = getDate();
	sys_req(WRITE, COM1, textrtc_landingclock, sizeof(textrtc_landingclock) + 5);
	puts(
		"\n$:Time: "
	);
	char* textrtc_landingtime = getTime();
	sys_req(WRITE, COM1, textrtc_landingtime, sizeof(textrtc_landingtime) + 4);

	curr_process = 000;
	return;
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
		"\n$:Help Commands:"\
		"\n$:	1) shutdown"\
		"\n$:	2) version"\
		"\n$:	3) jb"\
		"\n$:	4) rtc"\
		"\n$:	"\
		"\n\n"
	);
	

	curr_process = 000;
	return;
}