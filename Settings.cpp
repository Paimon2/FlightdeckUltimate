#include "Settings.h"
#include <XPLMUtilities.h>
#include <XPLMDataAccess.h>
#include <filesystem>
#include <string>
#include <sstream>
#include "Recognizer.h"

std::string voicePackPath;

void findAndReplaceAll_(std::string & data, std::string toSearch, std::string replaceStr)
{
	// Get the first occurrence
	size_t pos = data.find(toSearch);

	// Repeat till end is reached
	while (pos != std::string::npos)
	{
		// Replace this occurrence of Sub String
		data.replace(pos, toSearch.size(), replaceStr);
		// Get the next occurrence from the current position
		pos = data.find(toSearch, pos + toSearch.size());
	}
}

std::string getJsonPath() {

	std::string jsonPath;
	jsonPath.clear();
	jsonPath.append("Resources").append(dirSep).append("plugins").append(dirSep).append("Flightdeck Ultimate").append(dirSep).append("acfprofiles").append(dirSep);

	char acfICAO[1024];
	int ByteVals[500];
	XPLMGetDatab(XPLMFindDataRef("sim/aircraft/view/acf_ICAO"), ByteVals, 0, 1024);
	strcpy(acfICAO, (char*)&ByteVals);

	// Figure out our aircraft
	

	for (auto & p : std::experimental::filesystem::directory_iterator(jsonPath)) {

		std::ostringstream oss;
		oss << p;
		std::string fileName = oss.str();

		if (fileName.find(acfICAO) != std::string::npos) {
			findAndReplaceAll_(fileName, "Resources\\plugins\\Flightdeck Ultimate\\acfprofiles\\", "");
			jsonPath.append(fileName);
			return jsonPath;
		}
	}

	jsonPath.append("default.json");
	return jsonPath;
}


void initVoicePackPath(std::string voicepackname) {
	voicePackPath.clear();
	voicePackPath.append("Resources").append(dirSep).append("plugins").append(dirSep).append("Flightdeck Ultimate").append(dirSep).append("voices").append(dirSep).append(voicepackname).append(dirSep);
}