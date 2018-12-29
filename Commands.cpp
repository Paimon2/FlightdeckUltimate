#include "Commands.h"
#include "Recognizer.h"

XPLMCommandRef pttCommand = NULL;

int commandHandler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon)
{
	if (inPhase == xplm_CommandBegin)
	{
		defaultRecognizer.startListening();
	}

	
	if (inPhase == xplm_CommandContinue)
	{
		
	}

	
	if (inPhase == xplm_CommandEnd)
	{
		defaultRecognizer.stopListening();
	}

	return 0;
}

void initCommands() {

	pttCommand = XPLMCreateCommand("transonicsimulations/flightdeckultimate/ptt", "Push this key or button to talk to your virtual copilot!");

	XPLMRegisterCommandHandler(pttCommand,              // in Command name
		commandHandler,       // in Handler
		1,                      // Receive input before plugin windows.
		(void *)0);            // inRefcon.

	
}


void closeCommands() {

	XPLMUnregisterCommandHandler(pttCommand, commandHandler, 0, 0);

}