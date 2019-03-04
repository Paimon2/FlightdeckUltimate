#pragma once
#include <exception>
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <string>
#ifdef IBM
#include <windows.h>
#endif
#include <string>
#include <sstream>

#ifdef IBM
std::string getClipboardText()
{
	// Try opening the clipboard
	if (!OpenClipboard(nullptr))
		return "";

	  // Get handle of clipboard object for ANSI text
	HANDLE hData = GetClipboardData(CF_TEXT);
	if (hData == nullptr)
		return "";

	  // Lock the handle to get the actual text pointer
		char * pszText = static_cast<char*>(GlobalLock(hData));
	if (pszText == nullptr)
		return "";

	  // Save text in a string class instance
		std::string text(pszText);

	// Release the lock
	GlobalUnlock(hData);

	// Release the clipboard
	CloseClipboard();

	return text;
}
#endif

#ifdef APL
std::string exec(const char* cmd)
{
	FILE* pipe = popen(cmd, "r");
	if (!pipe) return "ERROR";
	char buffer[128];
	std::string result = "";
	while (!feof(pipe))
	{
		if (fgets(buffer, 128, pipe) != NULL)
		{
			result += buffer;
		}
	}
	pclose(pipe);
	return result;
}

std::string getClipboardText()
{
	return exec("pbpaste");
}

std::string copy(const char * new_clipboard)
{
	std::stringstream cmd;
	cmd << "echo \"" << new_clipboard << "\" | pbcopy";
	return exec(cmd.str().c_str());
}
#endif


