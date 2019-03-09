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

static XPLMWindowID MsgWindow;
Button okButton("               OK", 80, 20);
static char* message;

void MsgDraw(XPLMWindowID inWindowID, void * inRefcon) {
	int x, y;
	int mouse_x, mouse_y;
	int mIndex;
	int poppedOut = XPLMWindowIsPoppedOut(inWindowID);
	XPLMGetWindowGeometry(inWindowID, &x, nullptr, nullptr, &y);
	XPLMGetMouseLocationGlobal(&mouse_x, &mouse_y);

	XPLMDrawString(white, x, y + 150, message, 0, xplmFont_Proportional);
	okButton.draw(mouse_x, mouse_y, x, y);
	

	

}



void MsgKeystroke(XPLMWindowID in_window_id, char key, XPLMKeyFlags flags, char virtual_key, void * in_refcon, int losing_focus) {


}



int	MsgMouse(XPLMWindowID in_window_id, int x, int y, XPLMMouseStatus is_down, void * in_refcon)
{

	int wx, wy;
	XPLMGetWindowGeometry(in_window_id, &wx, nullptr, nullptr, &wy);

	okButton.onMouseEvent(x, y, is_down, wx, wy);

	return 1;
}


void createMsgWindow(const char* title, const char* msg) {



	int sw, sh;
	XPLMGetScreenSize(&sw, &sh);
	XPLMDestroyWindow(MsgWindow);
	XPLMCreateWindow_t MsgWindowST;
	MsgWindowST.structSize = sizeof(MsgWindowST);
	MsgWindowST.left = (sw / 2) - 150; // RHS = width / 2
	MsgWindowST.bottom = (sh / 2) - 125; // RHS = height / 2
	MsgWindowST.right = (sw / 2) + 150; // RHS = width / 2
	MsgWindowST.top = (sh / 2) + 125; // RHS = height / 2
	
	MsgWindowST.visible = 1;
	MsgWindowST.drawWindowFunc = MsgDraw;
	MsgWindowST.handleMouseClickFunc = MsgMouse;
	MsgWindowST.handleRightClickFunc = MsgMouse;
	MsgWindowST.handleMouseWheelFunc = dummy_wheel_handler;
	MsgWindowST.handleKeyFunc = MsgKeystroke;
	MsgWindowST.handleCursorFunc = dummy_cursor_status_handler;
	MsgWindowST.refcon = nullptr;
	MsgWindowST.layer = xplm_WindowLayerFloatingWindows;
	MsgWindowST.decorateAsFloatingWindow = 1;

	MsgWindow = XPLMCreateWindowEx(&MsgWindowST);
	XPLMSetWindowPositioningMode(MsgWindow, xplm_WindowPositionFree, -1);
	XPLMSetWindowGravity(MsgWindow, 0, 1, 0, 1);
//	XPLMSetWindowResizingLimits(MsgWindow, 300, 250, 300, 250);
	XPLMSetWindowTitle(MsgWindow, title);


	message = const_cast<char*>(msg);

	okButton.setCallback([]() {
		XPLMDestroyWindow(MsgWindow);
	});

}