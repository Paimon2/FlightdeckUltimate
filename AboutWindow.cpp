#include <thread>
#include "GUI.h"
#include "glfunctions.h"
#include "XPLMDisplay.h"
#include "Widgets.h"
#include "GlobalVars.h"
#include "Settings.h"
#include "Networking.h"
#include <iostream>
#include "fixed_string.hpp"
#include <fstream>
#include <filesystem>
#include <sstream>

static XPLMWindowID AboutWindow;

enum FadeState {
	Up = 0,
	Down = 1,
};

FadeState fstate;
int gg = 0;
int rr = 0;
int bb = 0;
fixed_string<512> updateText = "Checking for updates...";

void checkUpdatesAboutWindow() {
	updateText = "Checking for updates...";
	updateText = checkForUpdates();
}
void AboutDraw(XPLMWindowID inWindowID, void * inRefcon) {
	int x, y;
	int mouse_x, mouse_y;
	int mIndex;
	int poppedOut = XPLMWindowIsPoppedOut(inWindowID);
	XPLMGetWindowGeometry(inWindowID, &x, nullptr, nullptr, &y);
	XPLMGetMouseLocationGlobal(&mouse_x, &mouse_y);


	fstate == FadeState::Up ? gg++ : gg--;
	fstate == FadeState::Up ? bb++ : bb--;
	fstate == FadeState::Up ? rr++ : rr--;

	/*As && framesToHoldFor == 0 rr, gg and bb are all the same, we only need to use one variable to determine the saturation.*/
	if (gg > 254) { fstate = FadeState::Down; }
	if (gg < 1) { fstate = FadeState::Up; }

	drawTextureRGB(x + 25, y + 200, 400, 110, ts_logo, rr, gg, bb, 255);

	XPLMDrawString(white, x + 75, y + 170, fixed_string<64>("Flightdeck Ultimate - Version " VERSION_S), NULL, xplmFont_Proportional);
	XPLMDrawString(white, x + 75, y + 150, fixed_string<256>(_DEBUG ? "Debug version compiled on " __DATE__ :
	"Release version compiled on" __DATE__ ), NULL, xplmFont_Proportional);
	XPLMDrawString(white, x + 75, y + 120, fixed_string<32>("(C) 2019 Transonic Simulations"), NULL, xplmFont_Proportional);

	XPLMDrawString(white, x + 75, y + 100, updateText, NULL, xplmFont_Proportional);

}



void AboutKeystroke(XPLMWindowID in_window_id, char key, XPLMKeyFlags flags, char virtual_key, void * in_refcon, int losing_focus) {


}



int	AboutMouse(XPLMWindowID in_window_id, int x, int y, XPLMMouseStatus is_down, void * in_refcon)
{

	int mouse_x, mouse_y;
	XPLMGetWindowGeometry(in_window_id, &x, nullptr, nullptr, &y);
	XPLMGetMouseLocationGlobal(&mouse_x, &mouse_y);


	
	return 1;
}


void createAboutWindow() {



	int sw, sh;
	XPLMGetScreenSize(&sw, &sh);
	XPLMDestroyWindow(AboutWindow);
	XPLMCreateWindow_t AboutWindowST;
	AboutWindowST.structSize = sizeof(AboutWindowST);
	AboutWindowST.left = (sw / 2) - 200; // RHS = width / 2
	AboutWindowST.bottom = (sh / 2) - 150; // RHS = height / 2
	AboutWindowST.right = (sw / 2) + 200; // RHS = width / 2
	AboutWindowST.top = (sh / 2) + 150; // RHS = height / 2
	AboutWindowST.visible = 1;
	AboutWindowST.drawWindowFunc = AboutDraw;
	AboutWindowST.handleMouseClickFunc = AboutMouse;
	AboutWindowST.handleRightClickFunc = AboutMouse;
	AboutWindowST.handleMouseWheelFunc = dummy_wheel_handler;
	AboutWindowST.handleKeyFunc = AboutKeystroke;
	AboutWindowST.handleCursorFunc = dummy_cursor_status_handler;
	AboutWindowST.refcon = nullptr;
	AboutWindowST.layer = xplm_WindowLayerFloatingWindows;
	AboutWindowST.decorateAsFloatingWindow = 1;

	AboutWindow = XPLMCreateWindowEx(&AboutWindowST);
	XPLMSetWindowPositioningMode(AboutWindow, xplm_WindowPositionFree, -1);
	XPLMSetWindowGravity(AboutWindow, 0, 1, 0, 1);
	XPLMSetWindowResizingLimits(AboutWindow, 400, 300, 400, 300);
	XPLMSetWindowTitle(AboutWindow, "About Flightdeck Ultimate");

	std::thread checkForUpdateThread(checkUpdatesAboutWindow);
	checkForUpdateThread.detach();
	
	
}