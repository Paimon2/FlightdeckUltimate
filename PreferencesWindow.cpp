#include <thread>
#include "GUI.h"
#include "glfunctions.h"
#include "XPLMDisplay.h"
#include "Widgets.h"
#include "GlobalVars.h"
#include "Settings.h"
#include <iostream>
#include "fixed_string.hpp"
#include <fstream>
#include <filesystem>
#include <sstream>

namespace fs = std::experimental::filesystem;
constexpr short CHECKBOX_WIDTH_OFFSET = 200;

Button cancelButton("    Cancel", 80, 35, 75);
Button saveButton("      Save", 215, 35, 75);
Button resetButton("  Restore defaults", 130, 5, 113);

Slider soundLevel(20, 430, 80, 60);

DropdownMenu voicesDropdown(200, 275, 100, 18);
DropdownMenu recognitionMode(200, 345, 150, 18);

Checkbox checkUpdates(CHECKBOX_WIDTH_OFFSET, 550);
Checkbox readBackMsgs(CHECKBOX_WIDTH_OFFSET, 520);
Checkbox enable3DSound(CHECKBOX_WIDTH_OFFSET, 410);

Checkbox callout60(CHECKBOX_WIDTH_OFFSET, 210);
Checkbox callout80(CHECKBOX_WIDTH_OFFSET, 180);
Checkbox calloutV1(CHECKBOX_WIDTH_OFFSET, 150);
Checkbox calloutVr(CHECKBOX_WIDTH_OFFSET, 120);
Checkbox calloutV2(CHECKBOX_WIDTH_OFFSET, 90);

static XPLMWindowID PrefsWindow;

void PrefsDraw(XPLMWindowID inWindowID, void * inRefcon) {
	int x, y;
	int mouse_x, mouse_y;
	int mIndex;
	int poppedOut = XPLMWindowIsPoppedOut(inWindowID);
	XPLMGetWindowGeometry(inWindowID, &x, nullptr, nullptr, &y);
	XPLMGetMouseLocationGlobal(&mouse_x, &mouse_y);

	XPLMDrawString(tpr, x + 10, y + 580, fixed_string<32>("General"), NULL, xplmFont_Proportional);

	XPLMDrawString(white, x + 10, y + 550, fixed_string<33>("Check for updates automatically"), NULL, xplmFont_Proportional);
	checkUpdates.draw(x, y);

	XPLMDrawString(white, x + 10, y + 520, fixed_string<32>("Read back messages"), NULL, xplmFont_Proportional);
	readBackMsgs.draw(x, y);

	XPLMDrawString(tpr, x + 10, y + 480, fixed_string<32>("Audio"), NULL, xplmFont_Proportional);


	XPLMDrawString(white, x + 10, y + 430, fixed_string<32>("Output volume"), NULL, xplmFont_Proportional);
	XPLMDrawString(white, x + 10, y + 410, fixed_string<32>("Enable 3D sound"), NULL, xplmFont_Proportional);
	enable3DSound.draw(x, y);

	XPLMDrawString(tpr, x + 10, y + 380, fixed_string<32>("Recognition"), NULL, xplmFont_Proportional);

	XPLMDrawString(white, x + 10, y + 350, fixed_string<32>("Select recognition mode:"), NULL, xplmFont_Proportional);

	XPLMDrawString(tpr, x + 10, y + 310, fixed_string<32>("Voices"), NULL, xplmFont_Proportional);
	

	XPLMDrawString(white, x + 10, y + 280, fixed_string<32>("Select voice:"), NULL, xplmFont_Proportional);
	

	XPLMDrawString(tpr, x + 10, y + 240, fixed_string<32>("Callouts"), NULL, xplmFont_Proportional);

	XPLMDrawString(white, x + 10, y + 210, fixed_string<32>("60 knots"), NULL, xplmFont_Proportional);
	callout60.draw(x, y);

	XPLMDrawString(white, x + 10, y + 180, fixed_string<32>("80 knots"), NULL, xplmFont_Proportional);
	callout80.draw(x, y);

	XPLMDrawString(white, x + 10, y + 150, fixed_string<32>("V1"), NULL, xplmFont_Proportional);
	calloutV1.draw(x, y);

	XPLMDrawString(white, x + 10, y + 120, fixed_string<32>("Vr"), NULL, xplmFont_Proportional);
	calloutVr.draw(x, y);

	XPLMDrawString(white, x + 10, y + 90, fixed_string<32>("V2"), NULL, xplmFont_Proportional);
	calloutV2.draw(x, y);

	
	voicesDropdown.draw(mouse_x, mouse_y, x, y); /*draw at the bottom as it extends*/
	recognitionMode.draw(mouse_x, mouse_y, x, y); /*draw at the bottom as it extends*/
	soundLevel.draw(mouse_x, mouse_y, x, y);
	cancelButton.draw(mouse_x, mouse_y, x, y);
	saveButton.draw(mouse_x, mouse_y, x, y);
	resetButton.draw(mouse_x, mouse_y, x, y);
	
}



void PrefsKeystroke(XPLMWindowID in_window_id, char key, XPLMKeyFlags flags, char virtual_key, void * in_refcon, int losing_focus) {


}



int	PrefsMouse(XPLMWindowID in_window_id, int x, int y, XPLMMouseStatus is_down, void * in_refcon)
{

	int mouse_x, mouse_y;
	XPLMGetWindowGeometry(in_window_id, &x, nullptr, nullptr, &y);
	XPLMGetMouseLocationGlobal(&mouse_x, &mouse_y);

	checkUpdates.onMouseEvent(mouse_x, mouse_y, is_down, x, y);
	readBackMsgs.onMouseEvent(mouse_x, mouse_y, is_down, x, y);
	enable3DSound.onMouseEvent(mouse_x, mouse_y, is_down, x, y);
	voicesDropdown.onMouseEvent(mouse_x, mouse_y, is_down, x, y);
	recognitionMode.onMouseEvent(mouse_x, mouse_y, is_down, x, y);
	callout60.onMouseEvent(mouse_x, mouse_y, is_down, x, y);
	callout80.onMouseEvent(mouse_x, mouse_y, is_down, x, y);
	calloutV1.onMouseEvent(mouse_x, mouse_y, is_down, x, y);
	calloutVr.onMouseEvent(mouse_x, mouse_y, is_down, x, y);
	calloutV2.onMouseEvent(mouse_x, mouse_y, is_down, x, y);


	if (saveButton.onMouseEvent(mouse_x, mouse_y, is_down, x, y)) {

	}
	if (cancelButton.onMouseEvent(mouse_x, mouse_y, is_down, x, y)) {

	}
	if (resetButton.onMouseEvent(mouse_x, mouse_y, is_down, x, y)) {
	}

	return 1;
}


void createPrefsWindow() {
	voicesDropdown.addItem("Joe");

	recognitionMode.addItem("Continuous");
	recognitionMode.addItem("Push-to-talk");





	int sw, sh;
	XPLMGetScreenSize(&sw, &sh);
	XPLMDestroyWindow(PrefsWindow);
	XPLMCreateWindow_t PrefsWindowST;
	PrefsWindowST.structSize = sizeof(PrefsWindowST);
	PrefsWindowST.left = sw  / 2- 200; // x position
	PrefsWindowST.bottom = sh / 2 - 250; // height
	PrefsWindowST.right = sh / 2; // width
	PrefsWindowST.top = sw / 2 + 250; // y position
	PrefsWindowST.visible = 1;
	PrefsWindowST.drawWindowFunc = PrefsDraw;
	PrefsWindowST.handleMouseClickFunc = PrefsMouse;
	PrefsWindowST.handleRightClickFunc = PrefsMouse;
	PrefsWindowST.handleMouseWheelFunc = dummy_wheel_handler;
	PrefsWindowST.handleKeyFunc = PrefsKeystroke;
	PrefsWindowST.handleCursorFunc = dummy_cursor_status_handler;
	PrefsWindowST.refcon = nullptr;
	PrefsWindowST.layer = xplm_WindowLayerFloatingWindows;
	PrefsWindowST.decorateAsFloatingWindow = 1;

	PrefsWindow = XPLMCreateWindowEx(&PrefsWindowST);
	XPLMSetWindowPositioningMode(PrefsWindow, xplm_WindowPositionFree, -1);
	XPLMSetWindowGravity(PrefsWindow, 0, 1, 0, 1);
	XPLMSetWindowResizingLimits(PrefsWindow, 400, 600, 400, 600);
	XPLMSetWindowTitle(PrefsWindow, "User Preferences");


}