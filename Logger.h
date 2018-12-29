#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <fstream>

class Logger {
public:
	 std::ofstream logFileHandle;


	void initialize(std::string logFileName = "Flightdeck Ultimate.log");
	void close();

	
	void operator <<(std::string rhs);

	void operator <<(char* rhs);

	void logError(std::string msg);

	void logWarning(std::string msg);
};

