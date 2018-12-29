#pragma once
#include "libcurl/curl.h"
#include "GlobalVars.h"
#include "fixed_string.hpp"

struct ResponseInfo {
	std::string responseData;
	CURLcode responseCode;
};

extern ResponseInfo downloadToString(std::string URL);
extern fixed_string<512> checkForUpdates();