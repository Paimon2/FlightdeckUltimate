#include <string>
#include "XPLMMenus.h"
#include "XPLMPlugin.h"
#include "GUI.h"
#include "ChecklistEditorWindow.hpp"

using namespace std;

int  MainMenuContainer = NULL;
int UtilMenu = NULL;
int ChecklistMenu = NULL;
XPLMMenuID  UtilMenuID = NULL;
XPLMMenuID BeltMenuID = NULL;
XPLMMenuID ChecklistMenuID = NULL;
int beltMenu = NULL;
int HelpMenu = NULL;
XPLMMenuID  HelpMenuID = NULL;
XPLMMenuID MainMenuID = NULL;


void ChecklistMenuHandler(void*, void*);
void menu_handler(void*, void*);

void DestroyMenus() { if (MainMenuID != NULL) { XPLMDestroyMenu(MainMenuID); } } // Call this in XPluginStop.
void InitMenus() { MainMenuContainer = XPLMAppendMenuItem(XPLMFindPluginsMenu(), "Flightdeck Ultimate", 0, 0); } // *ONLY TO BE CALLED IN XPLUGINSTART!*


void CreateMenus()
{

	//DestroyMenus();
	MainMenuID = XPLMCreateMenu("Flightdeck Ultimate", XPLMFindPluginsMenu(), MainMenuContainer, menu_handler, NULL);
	XPLMAppendMenuItem(MainMenuID, "Enable Flightdeck Ultimate", (void *)"Enable Flightdeck Ultimate", 0);
	
	XPLMAppendMenuSeparator(MainMenuID);
	ChecklistMenu = XPLMAppendMenuItem(MainMenuID, "Checklists", 0, 0);
	ChecklistMenuID = XPLMCreateMenu("Checklists", MainMenuID, ChecklistMenu, ChecklistMenuHandler, NULL);
	XPLMAppendMenuItem(ChecklistMenuID, "Before start checklist", (void*)"Before start checklist", NULL);
	XPLMAppendMenuItem(ChecklistMenuID, "Before takeoff checklist", (void*)"Before takeoff checklist", NULL);
	XPLMAppendMenuItem(ChecklistMenuID, "Before taxi checklist", (void*)"Before taxi checklist", NULL);
	XPLMAppendMenuItem(ChecklistMenuID, "Approach checklist", (void*)"Approach checklist", NULL);
	XPLMAppendMenuItem(ChecklistMenuID, "Descent checklist", (void*)"Descent checklist", NULL);
	XPLMAppendMenuItem(ChecklistMenuID, "Landing checklist", (void*)"Landing checklist", NULL);
	XPLMAppendMenuSeparator(MainMenuID);

	XPLMAppendMenuItem(MainMenuID, "User preferences", (void *)"User preferences", 1);
	XPLMAppendMenuItem(MainMenuID, "Checklist editor", (void *)"Checklist editor", 1);
	XPLMAppendMenuItem(MainMenuID, "Reload aircraft profile", (void *)"Reload aircraft profile", 1);
	XPLMAppendMenuItem(MainMenuID, "Restart recognition engine", (void *)"Restart recognition engine", 1);
	XPLMAppendMenuSeparator(MainMenuID);
	XPLMAppendMenuItem(MainMenuID, "Help and support", (void *)"Help and Support", 1);
	XPLMAppendMenuItem(MainMenuID, "About Flightdeck Ultimate", (void *)"About Flightdeck Ultimate", 1);

}


void menu_handler(void * in_menu_ref, void * in_item_ref)
{
	
	if (!strcmp((const char *)in_item_ref, "User preferences")) {
//		createPrefsWindow();
	}

	if (!strcmp((const char *)in_item_ref, "Checklist editor")) {
		createChecklistEditorWindow();
	}


	if (!strcmp((const char *)in_item_ref, "About Flightdeck Ultimate")) {
		createAboutWindow();
	}


	


}


void ChecklistMenuHandler(void * in_menu_ref, void * in_item_ref)
{


}
