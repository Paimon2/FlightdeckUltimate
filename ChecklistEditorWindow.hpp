#pragma once
#ifndef _CHECKLIST_EDITOR_WINDOW_HPP_
#define _CHECKLIST_EDITOR_WINDOW_HPP_

#include "GUI.h"
#include "Widgets.h"
#include "glfunctions.h"
#include "XPLMDisplay.h"
#include "XPLMDataAccess.h"
#include "NativeFileDialog/nfd.h"
#include "json.hpp"
#include <fstream>
#include "GlobalVars.h"

static XPLMWindowID checklistEditorWindowHandle;
static nlohmann::json currentCeJsonHandle;

Button ceOpen("           Open", 83	, 560);
Button ceClose("         Close", 570, 560);
Button ceSave("           Save", 257, 560);
Button ceHelp("           Help", 420, 560);
TextBox phraseList(200, 200, 200, 25, 128U, 2, false, 1);

inline int checklistEditorMouseInput(XPLMWindowID inWindowID,
	int x,
	int y,
	XPLMMouseStatus is_down,
	void * in_refcon) 
{
	int wx, wy;
	XPLMGetWindowGeometry(inWindowID, &wx, nullptr, nullptr, &wy);

	ceOpen.onMouseEvent(x, y, is_down, wx, wy);
	ceSave.onMouseEvent(x, y, is_down, wx, wy);
	ceClose.onMouseEvent(x, y, is_down, wx, wy);
	ceHelp.onMouseEvent(x, y, is_down, wx, wy);
	if (phraseList.onMouseEvent(x, y, wx, wy)) {
		XPLMTakeKeyboardFocus(inWindowID);
	}

	return 1;

	}

inline void checklistEditorKeyInput(XPLMWindowID in_window_id,
	char key,
	XPLMKeyFlags flags,
	char virtual_key,
	void * in_refcon,
	int losing_focus) {
	phraseList.sendKey(key, virtual_key);
	}

inline void drawChecklistEditorWindow(XPLMWindowID         inWindowID,
										void *               inRefcon)
{
	int mouse_x, mouse_y;
	int x, y;
	XPLMGetMouseLocationGlobal(&mouse_x, &mouse_y);
	XPLMGetWindowGeometry(inWindowID, &x, nullptr, nullptr, &y);

	ceOpen.draw(mouse_x, mouse_y, x, y);
	ceSave.draw(mouse_x, mouse_y, x, y);
	ceClose.draw(mouse_x, mouse_y, x, y);
	ceHelp.draw(mouse_x, mouse_y, x, y);
	phraseList.draw(x, y);
}



inline void createChecklistEditorWindow() {
	
	

	int sw, sh;
	XPLMGetScreenSize(&sw, &sh);
	XPLMDestroyWindow(checklistEditorWindowHandle);
	XPLMCreateWindow_t EditorWindowST;
	EditorWindowST.structSize = sizeof(EditorWindowST);
	EditorWindowST.left = (sw / 2) - 400; // RHS = width / 2
	EditorWindowST.bottom = (sh / 2) - 300; // RHS = height / 2
	EditorWindowST.right = (sw / 2) + 400; // RHS = width / 2
	EditorWindowST.top = (sh / 2) + 300; // RHS = height / 2
	EditorWindowST.visible = 1;
	EditorWindowST.drawWindowFunc = drawChecklistEditorWindow;
	EditorWindowST.handleMouseClickFunc = checklistEditorMouseInput;
	EditorWindowST.handleRightClickFunc = checklistEditorMouseInput;
	EditorWindowST.handleMouseWheelFunc = dummy_wheel_handler;
	EditorWindowST.handleKeyFunc = checklistEditorKeyInput;
	EditorWindowST.handleCursorFunc = dummy_cursor_status_handler;
	EditorWindowST.refcon = nullptr;
	EditorWindowST.layer = xplm_WindowLayerFloatingWindows;
	EditorWindowST.decorateAsFloatingWindow = 1;

	checklistEditorWindowHandle = XPLMCreateWindowEx(&EditorWindowST);
	XPLMSetWindowPositioningMode(checklistEditorWindowHandle, xplm_WindowPositionFree, -1);
	XPLMSetWindowGravity(checklistEditorWindowHandle, 0, 1, 0, 1);
	XPLMSetWindowResizingLimits(checklistEditorWindowHandle, 800, 600, 800, 600);
	XPLMSetWindowTitle(checklistEditorWindowHandle, "Checklist editor");

	ceSave.setCallback([]() {

		

	});

	ceOpen.setCallback([]() {

		int width, height;
		XPLMGetScreenSize(&width, &height);
		int screenWidth, screenHeight;
		getScreenResolution(screenWidth, screenHeight);


		if (screenWidth == width
			&& screenHeight == height) {
			createMsgWindow("Fullscreen mode not supported",
				"  Please exit fullscreen mode to open a checklist file.");
			return;
		}

		nfdchar_t *outPath = NULL;
		nfdresult_t result = NFD_OpenDialog("json", NULL, &outPath);

		if (result == NFD_OKAY) {
			std::ifstream jsonStream(outPath);
			jsonStream >> currentCeJsonHandle;
		}

		else if (result == NFD_ERROR) {
			pluginLogger.logError("NFD open file dialog error:");
			pluginLogger.logError(NFD_GetError());
		}

	});
}



#endif
