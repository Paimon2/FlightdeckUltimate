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
#include <iomanip>
#include "GlobalVars.h"
#include <experimental/filesystem>
#include <sstream>

static XPLMWindowID checklistEditorWindowHandle;
static nlohmann::json currentCeJsonHandle;
static nfdchar_t *outPath = NULL;

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

	if (phraseList.onMouseEvent(x, y, wx, wy)
		|| CoordGUIInRect(x, y, wx, wy, 800, 600)
		) {
		XPLMTakeKeyboardFocus(inWindowID);
	}

	else {
		XPLMTakeKeyboardFocus(0);
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

	// CTRL + S
	if (key == 19) {
		ceSave.callback();
		XPLMTakeKeyboardFocus(in_window_id);
	}

	// CTRL + O
	if (key == 15) {
		ceOpen.callback();
		XPLMTakeKeyboardFocus(in_window_id);
	}

	// F1 (commonly used as help)
	if (key == 112) {
		ceHelp.callback();
		XPLMTakeKeyboardFocus(in_window_id);
	}


	}

inline void drawChecklistEditorWindow(XPLMWindowID         inWindowID,
										void *               inRefcon)
{
	int mouse_x, mouse_y;
	int x, y;
	XPLMGetMouseLocationGlobal(&mouse_x, &mouse_y);
	XPLMGetWindowGeometry(inWindowID, &x, nullptr, nullptr, &y);

#pragma region Top buttons


	ceOpen.draw(mouse_x, mouse_y, x, y);

	if (CoordGUIInRect(mouse_x, mouse_y, 
		x + ceOpen.x, y + ceOpen.y, 150, 25))
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor4ub(150, 150, 150, 180);
		drawRectangle(mouse_x, mouse_y, mouse_x - 200, mouse_y - 60);
		XPLMDrawString(white, mouse_x - 180, mouse_y - 20, fixed_string<22>("Open a checklist file"), 0, xplmFont_Proportional);
		XPLMDrawString(tpr, mouse_x - 180, mouse_y - 40, fixed_string<16>("CTRL + O"), 0, xplmFont_Proportional);
	}

	ceSave.draw(mouse_x, mouse_y, x, y);
	if (CoordGUIInRect(mouse_x, mouse_y,
		x + ceSave.x, y + ceSave.y, 150, 25))
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor4ub(150, 150, 150, 180);
		drawRectangle(mouse_x, mouse_y, mouse_x - 200, mouse_y - 60);
		XPLMDrawString(white, mouse_x - 180, mouse_y - 20, fixed_string<25>("Save changes to the file"), 0, xplmFont_Proportional);
		XPLMDrawString(tpr, mouse_x - 180, mouse_y - 40, fixed_string<16>("CTRL + S"), 0, xplmFont_Proportional);
	}

	ceHelp.draw(mouse_x, mouse_y, x, y);
	if (CoordGUIInRect(mouse_x, mouse_y,
		x + ceHelp.x, y + ceHelp.y, 150, 25))
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor4ub(150, 150, 150, 180);
		drawRectangle(mouse_x, mouse_y, mouse_x - 250, mouse_y - 60);
		XPLMDrawString(white, mouse_x - 240, mouse_y - 20, fixed_string<41>("Get online help on how to use the editor"),
			0, xplmFont_Proportional);
		XPLMDrawString(tpr, mouse_x - 240, mouse_y - 40, fixed_string<3>("F1"), 0, xplmFont_Proportional);
	}

	ceClose.draw(mouse_x, mouse_y, x, y);
	if (CoordGUIInRect(mouse_x, mouse_y,
		x + ceClose.x, y + ceClose.y, 150, 25))
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor4ub(150, 150, 150, 180);
		drawRectangle(mouse_x, mouse_y, mouse_x - 220, mouse_y - 40);
		XPLMDrawString(white, mouse_x - 210, mouse_y - 20, fixed_string<35>("Exit the editor discarding changes"), 
			0, xplmFont_Proportional);
	}


#pragma endregion


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
	XPLMTakeKeyboardFocus(checklistEditorWindowHandle);

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

		#if IBM
		nfdresult_t result = NFD_OpenDialog("json", NULL, &outPath, 
			fixed_string<64>("Open aircraft configuration file"));
		#else
		nfdresult_t result = NFD_OpenDialog("json", NULL, &outPath);
		#endif
		if (result == NFD_OKAY) {
			std::ifstream jsonStream(outPath);
			jsonStream >> currentCeJsonHandle;
			std::string fileName;
			std::stringstream oss;
			oss << std::experimental::filesystem::path(outPath).filename();
			fileName = oss.str();
			XPLMSetWindowTitle(checklistEditorWindowHandle,
				fixed_string<256>("Checklist editor - " + fileName));
		}

		else if (result == NFD_ERROR) {
			pluginLogger.logError("NFD open file dialog error:");
			pluginLogger.logError(NFD_GetError());
		}

	});
}



#endif
