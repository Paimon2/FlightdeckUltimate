#include "Logger.h"

void Logger::initialize(std::string logFileName) {
	if (!logFileHandle.is_open()) {
		logFileHandle.open(logFileName, std::ios_base::trunc | std::ios_base::out);
		std::cout.rdbuf(logFileHandle.rdbuf()); //redirect std::cout
	}


}

void Logger::close() {
	if (logFileHandle.is_open()) {
		logFileHandle.close();
	}
}

void Logger::operator <<(std::string rhs) {
	std::string msgToLog = "";
	msgToLog = msgToLog + "[";
	time_t rawtime;
	struct tm * timeinfo;
	char buffer[80];
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	strftime(buffer, sizeof(buffer), "%d-%m-%Y %I:%M:%S", timeinfo);
	std::string str(buffer);
	msgToLog = msgToLog + str + "]";

	logFileHandle << msgToLog + " [INFO] " + rhs + "\n";

	logFileHandle.flush();
}

void Logger::operator<<(char * rhs)
{
	std::string msgToLog = "";
	msgToLog = msgToLog + "[";
	time_t rawtime;
	struct tm * timeinfo;
	char buffer[80];
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	strftime(buffer, sizeof(buffer), "%d-%m-%Y %I:%M:%S", timeinfo);
	std::string str(buffer);
	msgToLog = msgToLog + str + "]";

	logFileHandle << msgToLog + " [INFO] " + rhs + "\n";

	logFileHandle.flush();
}

void Logger::logError(std::string msg) {
	std::string msgToLog = "";
	msgToLog = msgToLog + "[";
	time_t rawtime;
	struct tm * timeinfo;
	char buffer[80];
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	strftime(buffer, sizeof(buffer), "%d-%m-%Y %I:%M:%S", timeinfo);
	std::string str(buffer);
	msgToLog = msgToLog + str + "]";

	logFileHandle << msgToLog + " [ERROR] " + msg + "\n";

	logFileHandle.flush();
}


void Logger::logWarning(std::string msg) {
	std::string msgToLog = "";
	msgToLog = msgToLog + "[";
	time_t rawtime;
	struct tm * timeinfo;
	char buffer[80];
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	strftime(buffer, sizeof(buffer), "%d-%m-%Y %I:%M:%S", timeinfo);
	std::string str(buffer);
	msgToLog = msgToLog + str + "]";

	logFileHandle << msgToLog + " [WARN] " + msg + "\n";

	logFileHandle.flush();
}