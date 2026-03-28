#include "FileLogger.h"
#include <ctime>
#include <iomanip>
#include <sstream>

FileLogger::FileLogger(const std::string& filename) {
	//файл в режиме перезаписи 
	file_.open(filename, std::ios::out);
	if (!file_.is_open()) {
		throw std::runtime_error("Не удалось открыть файл для логирования: " + filename);
	}
}

FileLogger::~FileLogger() {
	if (file_.is_open()) {
		file_.close();
	}
}

std::string FileLogger::getCurrentTime() const {
	auto now = std::time(nullptr);
	std::tm tm;
//перевод времени в локальное
#ifdef _WIN32
	localtime_s(&tm, &now);
#else
	localtime_r(&now, &tm);
#endif
	std::ostringstream oss;
	oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
	return oss.str();
}

void FileLogger::log(const std::string& message) {
	if (file_.is_open()) {
		file_ << "[" << getCurrentTime() << "] " << message << "\n";
		file_.flush();
	}
}

