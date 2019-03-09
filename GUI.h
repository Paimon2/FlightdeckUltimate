#pragma once
#include <string>
extern std::string getClipboardText();
extern void createChecklistEditorWindow();
extern void createCPWWindow();
extern void createAboutWindow();
extern void getScreenResolution(int &width, int &height);
extern void createMsgWindow(const char* title, const char* msg);