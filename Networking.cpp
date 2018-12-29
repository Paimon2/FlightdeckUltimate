#include <iostream>
#include <string>
#include "Networking.h"



static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
	((std::string*)userp)->append((char*)contents, size * nmemb);
	return size * nmemb;
}


ResponseInfo downloadToString(std::string URL)
{
	ResponseInfo tempResponse;
	CURL *curl;
	CURLcode res;
	std::string readBuffer;

	curl = curl_easy_init();
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, URL.c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(curl, CURLOPT_USERAGENT, "Flightdeck Ultimate TS libcurl/ncv");
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);

		
		tempResponse.responseCode = res;
		tempResponse.responseData = readBuffer;
		
	}

	return tempResponse;
}
 

fixed_string<512> checkForUpdates() {

	ResponseInfo res = downloadToString("http://pluginapi.transonicsimulations.com/query/update");

	if (res.responseCode != CURLE_OK) {
		pluginLogger.logError("Unable to check for updates. Response code was: " + std::to_string(res.responseCode));
		return "Unable to check for updates";
	}
		
	try {
		pluginLogger << "Update check response: " + res.responseData;
		int latestVersion = std::stoi(res.responseData);
		/* IMPORTANT IMPORTANT IMPORTANT IMPORTANT IMPORTANT IMPORTANT IMPORTANT IMPORTANT IMPORTANT
		Change JSON format from small int to big int SERVER-SIDE. Includes resetting Flask webserver.
		E.g version 10 = 100, version 1.2.5 = 125
		*/
		if (latestVersion > CURRENT_VERSION_N) {
			return "A newer version is available; download from vendor";
		}
		return "You are running the latest version";
	}
	

	catch (...) {
		pluginLogger.logError("Unable to check for updates (weird response format?).");
		return "Unable to check for updates";
	}


	
}