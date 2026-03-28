#include "ConsoleLogger.h"
#include <iostream>
#include <ctime>
#include <iomanip>
#include <sstream>

std::string ConsoleLogger::getCurrentTime() const {
	auto now = std::time(nullptr);
	std::tm tm;
#ifdef _WIN32
	localtime_s(&tm, &now);
#else
	localtime_r(&now, &tm);
#endif
	std::ostringstream oss;
	oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
	return oss.str();
}

void ConsoleLogger::log(const std::string& message) {
	std::cout << "[LOG " << getCurrentTime() << "] " << message << "\n";
}

