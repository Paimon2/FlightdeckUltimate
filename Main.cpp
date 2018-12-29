#include <XPLMDisplay.h>
#include <XPLMGraphics.h>
#include <XPLMUtilities.h>
#include <XPLMPlugin.h>
#include <XPLMProcessing.h>
#include <string>
#include <iostream>
#include "Checklist.h"
#include "Recognizer.h"
#include "Widgets.h"
#include "Menus.h"
#include "GlobalVars.h"
#include "Networking.h"
#include "Datarefs.h"
#include "Settings.h"
#include "Commands.h"
#include "GUI.h"



Logger pluginLogger;
int commandsPosition;
bool betterPushbackExists;
bool passengersFXExists;
bool commandsChecked;
char dirSep[2];


void initDataPath() { // Modification of FlyWithLua's directory grabbing system

	strcpy(dirSep, XPLMGetDirectorySeparator()); // correct for each OS

}

float halfSecondLogic(float, float, int, void *) {
	textBoxBlinker == 1 ? textBoxBlinker = 0 : textBoxBlinker = 1;

	return 0.5;
}

float RunEveryFrame(float, float, int, void *) {

	if (!commandsToExecute.empty()) {
		if (!commandsChecked) { commandsPosition = 0; commandsChecked = true; }
		XPLMCommandOnce(commandsToExecute.at(commandsPosition));

		if (commandsPosition + 1 > commandsToExecute.size())
		{	commandsPosition = 0; commandsChecked = false; }
		else { commandsPosition++; }
	}
	
	sf::Listener::setPosition(sf::Vector3f(XPLMGetDataf(datarefs::pos_x), XPLMGetDataf(datarefs::pos_y), XPLMGetDataf(datarefs::pos_z)));
	sf::Listener::setDirection(sf::Vector3f(XPLMGetDataf(datarefs::head_psi), -XPLMGetDataf(datarefs::head_the), -1.0f));

	

	if (!floatRefsToSet.empty()) {

		XPLMSetDataf(floatRefsToSet.front().handle, floatRefsToSet.front().value);
		floatRefsToSet.pop();
	}
	
	if (!intRefsToSet.empty()) {

		XPLMSetDatai(intRefsToSet.front().handle, intRefsToSet.front().value);
		intRefsToSet.pop();
	}

	//pluginLogger << std::to_string(soundsToPlay.size());
	

	if (!soundsToPlay.empty()) {

		/*
		if(!currentSound) {

			sf::Music thisSound;
			thisSound.openFromFile(soundsToPlay.at(currentSoundIndex));
			thisSound.setVolume(100); // todo custom volume with json settings 
			thisSound.setPitch(static_cast<float>((rand() % 115 + 65) / 100)); // slightly varying pitch for each word
			thisSound.setPosition(sf::Vector3f(0.66f, 1.59f, -19.64f));
			thisSound.play();
			sf::SoundStream::Status stat = thisSound.getStatus();
			while (stat != sf::SoundStream::Status::Stopped) {}
			//	currentSound = &thisSound;
			if (currentSoundIndex + 1 < soundsToPlay.size()) {
				currentSoundIndex++;
			}
			else {
				currentSoundIndex = 0;
				soundsToPlay.clear();
			}
		}*/
	}
	
	if (soundsToPlay.empty()) {
		currentSoundIndex = 0;
	}



	return -1;
}

float RunOnceAtStartup(float, float, int, void *) {

	createAboutWindow();

	return 0;
}

PLUGIN_API int XPluginStart(
	char *		outName,
	char *		outSig,
	char *		outDesc)
{

	strcpy(outName, "Flightdeck Ultimate");
	strcpy(outSig, "TransonicSimulations.FlightdeckUltimate");
	strcpy(outDesc, "An interactive, voice-controllable copilot for X-Plane.");

	

	return 1;

}



PLUGIN_API void	XPluginStop(void)
{

}

PLUGIN_API void XPluginDisable(void) {

	defaultRecognizer.closeRecognizer();
	XPLMUnregisterFlightLoopCallback(RunOnceAtStartup, NULL);
	XPLMUnregisterFlightLoopCallback(RunEveryFrame, NULL);
	XPLMUnregisterFlightLoopCallback(halfSecondLogic, NULL);
	closeCommands();

}


PLUGIN_API int  XPluginEnable(void) {


	XPLMEnableFeature("XPLM_USE_NATIVE_PATHS", 1);
	XPLMDebugString("Flightdeck Ultimate is now being loaded. Please see the Flightdeck-Ultimate.log file for output.\n");

	pluginLogger.initialize(); // initialize the logger

	initDataPath(); // init data path
	pluginLogger << "Initialized data path.";


	defaultRecognizer.initRecognizer(); // init the recognizer
	initCommands(); // init the commands
	pluginLogger << "Initialized commands.";

	initVoicePackPath(); // init the path to our voices
	loadGLTextures(); // load the opengl textures we want to use

	InitMenus(); 
	CreateMenus(); // what's with this naming convention inconsistency???
	pluginLogger << "Initialized menu system.";

	XPLMRegisterFlightLoopCallback(RunOnceAtStartup, -1.0, NULL);
	XPLMRegisterFlightLoopCallback(RunEveryFrame, -1.0, NULL);
	XPLMRegisterFlightLoopCallback(halfSecondLogic, 0.5, NULL);

	

	return 1;



}
PLUGIN_API void XPluginReceiveMessage(XPLMPluginID inFrom, int inMsg, void * inParam) {
	
	if (inMsg == XPLM_MSG_PLANE_LOADED) {
		if ((int)(inParam == 0)) {
			std::string jsonPath = getJsonPath();
			char callsign[1024];
			int ByteVals[500];
			XPLMGetDatab(XPLMFindDataRef("sim/aircraft/view/acf_ICAO"), ByteVals, 0, 1024);
			strcpy(callsign, (char*)&ByteVals);
			pluginLogger << "The aircraft being loaded is: " + std::string(callsign);

			try {
				std::ifstream json_if(jsonPath);
				json_if >> aircraftJson;

			}
			catch (nlohmann::json::parse_error &e) {
				std::string exMsg(e.what());
				pluginLogger << "JSON conversion to stream failed due to: " + exMsg;
			}

			if (jsonPath.find("default.json") == /*NOT a default aircraft*/ std::string::npos) {


				try {
					pluginLogger << "Attempting to parse before taxi checklist.";
					beforeTaxiChecklist.parseChecklistFromJSON(aircraftJson, "before_taxi_checklist");
					pluginLogger << "Attempting to parse before takeoff checklist.";
					beforeTakeoffChecklist.parseChecklistFromJSON(aircraftJson, "before_takeoff_checklist");
					pluginLogger << "Attempting to parse before start checklist.";
					beforeStartChecklist.parseChecklistFromJSON(aircraftJson, "before_start_checklist");
					pluginLogger << "Attempting to parse approach checklist.";
					approachChecklist.parseChecklistFromJSON(aircraftJson, "approach_checklist");
					pluginLogger << "Attempting to parse descent checklist.";
					descentChecklist.parseChecklistFromJSON(aircraftJson, "descent_checklist");
					pluginLogger << "Attempting to parse landing checklist.";
					landingChecklist.parseChecklistFromJSON(aircraftJson, "landing_checklist");
				}
				catch (nlohmann::json::parse_error &e) {
					pluginLogger.logError("JSON checklist parse error. The details are below.");
					pluginLogger << e.what();
				}
				catch (...) {
					pluginLogger.logError("Checklist data extraction failure!");
				}

			}

			initCommandsList();
			initDatarefsList();
		}
	}

}

