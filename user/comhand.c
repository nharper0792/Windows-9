#include <mpx/comhand.h>
#include <mpx/serial.h>
#include <memory.h>
#include <sys_req.h>
#include <string.h>

/*
Variable: process
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
int	process = 0;
//==
		//	REMOVE BEFORE SUBMISSION OF R1 -- FOR TESTING ONLY
		//	NREAD ASSIGNMENT ONLY FOR TESTING
		//	REMOVE BEFORE SUBMISSION OF R1 AND CHANGE ACCORDING CODE
int nread = 10;
//==
void init_comhand(void) {
	/*
	Variable: text[info,help,landing,...]
	Use     : mutates into text needed for needed prompt
	*/
	char textlanding[] = "\n$:You have arrived!:";
	sys_req(WRITE, COM1, textlanding, sizeof(textlanding));



	for (;;)
	{
		/*
		Variable: buf
		Use		: buffers packets of data from input devices

		Variable: nread
		Use		: takes a byte of input, to be processed
		*/

		//==
		// RECOMMENT BEFORE SUBMISSION OF R1 -- FOR TESTING ONLY
		//	char buf[100] = { 0 };
		//	int nread = serial_poll(COM1, buf, sizeof(buf));
		//	sys_req(WRITE, COM1, buf, nread);
		//==

		//==
		//	REMOVE BEFORE SUBMISSION OF R1 -- FOR TESTING ONLY
		//	THIS BUFFER AND NREAD ASSIGNMENT WILL BE A TEMPORARY TEST METHOD UNTIL BUFFER IS FINISHED IN SERIAL_POLL
		//	REMOVE BEFORE SUBMISSION OF R1 AND CHANGE ACCORDING CODE
		char buf[] = "help";
		//==



		//==
		//	SMALL TEXT GROUPS
		//		This area contains the command prompts that are simple text.
		//==

		char textversion[] = "version";
		//version
		//process 010
		if ((strcmp(textversion, buf) <= 0) && nread == 10) {
			process = 010;
			comhand_version();
		}

		char textshutdown[] = "shutdown";
		//shutdown
		//process 020
		if ((strcmp(textshutdown, buf) <= 0) && (nread == 10)) {
			process = 020;
			comhand_shutdown();
		}

		char textjb[] = "jb";
		//joe burrow
		//process 030
		if ((strcmp(textjb, buf) <= 0) && nread == 10) {
			process = 030;
			comhand_jb();
		}

		char texthelp[] = "help";
		//help
		//process 040
		if ((strcmp(texthelp, buf) <= 0) && nread == 10) {
			process = 040;
			comhand_help();
		}

		char textrtc[] = "rtc";
		//help
		//process 050
		if ((strcmp(textrtc, buf) <= 0) && nread == 10) {
			process = 050;
			comhand_rtc();
		}

		nread = 0;
	}
}

//==============================================================
//CONTAINS FUNCTIONS USED WITHIN FOREVER LOOP OF COMMAND HANDLER
//==============================================================

void comhand_version(void) {
	char textversion_landing[] = "\n$:Version:\n==Windows 9 JB Edition==\nVersion R1.0\n";
	sys_req(WRITE, COM1, textversion_landing, sizeof(textversion_landing));
	process = 0;
	return;
}

void comhand_shutdown(void) {
	char textshutdown_landing[] = "\n$:Shutting Down:";
	sys_req(WRITE, COM1, textshutdown_landing, sizeof(textshutdown_landing));

	//process 021 - shutting down prompt
	process = 021;
	char textshutdown_landingconfirmation[] =
		"\n$:Are you sure you want to shutdown?:"\
		"\n$:	1) Yes"\
		"\n$:	1) No";
	sys_req(WRITE, COM1, textshutdown_landingconfirmation, sizeof(textshutdown_landingconfirmation));
	sys_req(-1);
	sys_req(EXIT);
	return;
}

//comhand_jb() is a function used to test various features within the command handler. currently, it is being used to test user prompts from within another process.
void comhand_jb(void) {
	char textjb_landing[] =
		"\n"\
		"\n(JB)$:It's me, Joe Burrow! I throw footballs!:"\
		"\n(JB)$:What do you want to talk about?:";
	sys_req(WRITE, COM1, textjb_landing, sizeof(textjb_landing));

	char textjb_landing01[] = "steelers";
	//==
	// REMOVE
	char buf2[] = "steelers";
	// REMOVE
	//==
	if ((strcmp(textjb_landing01, buf2) <= 0) && (nread == 10) && (process = 030)) {
		char textjb_landing01response[] =
			"\n(JB)$:Did you mention the Steelers?"\
			"\n(JB)$:I'm not worried about those gremlins.\n";
		sys_req(WRITE, COM1, textjb_landing01response, sizeof(textjb_landing01response));

	}
	nread = 0;
	process = 0;
	return;
}

void comhand_rtc(void) {
	char textrtc_landing[] = "\n$:Real-Time Clock:";
	sys_req(WRITE, COM1, textrtc_landing, sizeof(textrtc_landing));

	return;
}

void comhand_help(void) {
	char texthelp_landing[] =
		"\n$:Help Commands:"\
		"\n$:	1) shutdown"\
		"\n$:	2) version"\
		"\n$:	3) jb"\
		"\n$:	4) "
		"\n";
	sys_req(WRITE, COM1, texthelp_landing, sizeof(texthelp_landing));
	process = 0;
	return;
}