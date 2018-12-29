#include <thread>
#include "GUI.h"
#include "glfunctions.h"
#include "GlobalVars.h"
#include "XPLMDisplay.h"
#include "Checklist.h"
#include "Widgets.h"
#include <iostream>
#include "fixed_string.hpp"

/*It might be a good idea to just put them here*/
Checklist currentChecklist;
Checklist beforeTaxiChecklist;
Checklist beforeTakeoffChecklist;
Checklist beforeStartChecklist;
Checklist approachChecklist;
Checklist descentChecklist;
Checklist landingChecklist;

static XPLMWindowID CPWWindow;
ChecklistItem currentItem;
float targetChecklistProgress; // how many pixels to render out of the total width (390) !!! TARGET ONLY
float checklistProgress; // how many pixels to render out of the total width (390)
float progressBarSpeed;

bool inChecklistMode;
Button skipButton(fixed_string<16>("   Skip / >>"), 315, 0, 75, 25);

void inc_() { currentChecklist.currentIndex++; } /*hacky fix*/
void inc() { 
	currentChecklist.currentIndex + 1 > currentChecklist.items.size() ? XPLMDestroyWindow(CPWWindow) : inc_();
XPLMSetWindowTitle(CPWWindow, fixed_string<64>(currentChecklist.checklistName
	+ " (" + std::to_string(currentChecklist.currentIndex + 1) + "/" + std::to_string(currentChecklist.items.size()) + ")"));
targetChecklistProgress = (static_cast<float>(currentChecklist.currentIndex) / static_cast<float>(currentChecklist.items.size())) * 390.0f;
progressBarSpeed = (targetChecklistProgress - checklistProgress) * 0.08f;
}
void nextChecklistItem() { ((currentChecklist.currentIndex + 1) > currentChecklist.items.size()) ? XPLMDestroyWindow(CPWWindow) : inc(); }


void CPWDraw(XPLMWindowID inWindowID, void * inRefcon) { // size is 400 x 75.
	int x, y;
	int mouse_x, mouse_y;
	int mIndex;
	int poppedOut = XPLMWindowIsPoppedOut(inWindowID);
	XPLMGetWindowGeometry(inWindowID, &x, nullptr, nullptr, &y);
	XPLMGetMouseLocationGlobal(&mouse_x, &mouse_y);

	if (checklistProgress < targetChecklistProgress) {
		checklistProgress += progressBarSpeed;
		progressBarSpeed -= 0.1f;
		if (progressBarSpeed < 0) { progressBarSpeed = 0.1f; }
	}

	fixed_string<24> readbackText = "Readback:";
	
	try {
		currentItem = currentChecklist.items.at(currentChecklist.currentIndex);
	}
	catch (std::out_of_range) {
		// do nothing - bad practice...
	}

	fixed_string<256> itemName = currentItem.itemName;
	fixed_string<256> itemResponse = currentItem.itemResponse;
	if (targetChecklistProgress >= 390) { 
		itemName = ""; itemResponse = "";
		readbackText = "Checklist complete"; inChecklistMode = false;
		XPLMSetWindowTitle(CPWWindow, fixed_string<64>(currentChecklist.checklistName));
	 }
	if (checklistProgress >= 390) {

		XPLMDestroyWindow(CPWWindow);
		targetChecklistProgress = 0;
		checklistProgress = 0;
	}

	// Draw button to skip current checklist item
	skipButton.draw(mouse_x, mouse_y, x, y);

	// Draw progress bar background
	XPLMSetGraphicsState(0, 0, 0, 0, 0, 1, 0); /*disable alpha blending*/
	glColor3ub(111, 111, 111);
	drawRectangle(x + 10, y + 35, x + 390, y + 55);
	
	

	XPLMDrawString(yellow, x + 10, y + 65, fixed_string<256>(itemName), NULL, xplmFont_Proportional);

	XPLMDrawString(tpr, x + 10, y + 20, readbackText, NULL, xplmFont_Proportional);
	XPLMDrawString(grey, x + 75, y + 20, fixed_string<256>(itemResponse), NULL, xplmFont_Proportional);

	
	// Draw actual progress overlay
	//XPLMSetGraphicsState(0, 0, 0, 0, 0, 1, 0); /*disable alpha blending*/
	glColor3ub(29, 160, 242);
	// ABOVE ARE TEMPORARY. REPLACE THEM WITH ACTUAL VALUES.
	drawRectangle(x + 10, y + 35, x + checklistProgress, y + 55);

}






int	CPWMouse(XPLMWindowID in_window_id, int x, int y, XPLMMouseStatus is_down, void * in_refcon)
{
	int mouse_x, mouse_y;
	XPLMGetWindowGeometry(in_window_id, &x, nullptr, nullptr, &y);
	XPLMGetMouseLocationGlobal(&mouse_x, &mouse_y);

	if(skipButton.onMouseEvent(mouse_x, mouse_y, is_down, x, y)) {
		nextChecklistItem();
	}


	return 1;
}


void createCPWWindow() {
	int sw, sh;
	XPLMGetScreenSize(&sw, &sh);
	XPLMDestroyWindow(CPWWindow);
	XPLMCreateWindow_t CPWWindowST;
	CPWWindowST.structSize = sizeof(CPWWindowST);
	CPWWindowST.left = sw - 420; // x position
	CPWWindowST.bottom = sh - 145; // height
	CPWWindowST.right = sh / 2; // width
	CPWWindowST.top = sw + 400; // y position
	CPWWindowST.visible = 1;
	CPWWindowST.drawWindowFunc = CPWDraw;
	CPWWindowST.handleMouseClickFunc = CPWMouse;
	CPWWindowST.handleRightClickFunc = CPWMouse;
	CPWWindowST.handleMouseWheelFunc = dummy_wheel_handler;
	CPWWindowST.handleKeyFunc = dummy_key_handler;
	CPWWindowST.handleCursorFunc = dummy_cursor_status_handler;
	CPWWindowST.refcon = nullptr;
	CPWWindowST.layer = xplm_WindowLayerFloatingWindows;
	CPWWindowST.decorateAsFloatingWindow = 1;

	CPWWindow = XPLMCreateWindowEx(&CPWWindowST);
	XPLMSetWindowPositioningMode(CPWWindow, xplm_WindowPositionFree, -1);
	XPLMSetWindowGravity(CPWWindow, 0, 1, 0, 1);
	XPLMSetWindowResizingLimits(CPWWindow, 400, 75, 400, 75);
	currentChecklist.currentIndex + 1 > currentChecklist.items.size() ? XPLMDestroyWindow(CPWWindow) : inc_();
	XPLMSetWindowTitle(CPWWindow, fixed_string<64>(currentChecklist.checklistName
		+ " (" + std::to_string(currentChecklist.currentIndex + 1) + "/" + std::to_string(currentChecklist.items.size()) + ")"));
	XPLMBringWindowToFront(CPWWindow);

	inChecklistMode = true;
}