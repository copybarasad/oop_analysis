#ifndef FILELOGGER_H
#define FILELOGGER_H

#include "Logger.h"
#include <fstream>
#include <string>

// Реализация Logger для записи в файл
class FileLogger : public Logger {
private:
	std::ofstream file_;
	std::string getCurrentTime() const;

public:
	FileLogger(const std::string& filename);
	~FileLogger();
	void log(const std::string& message) override;
};

#endif 





