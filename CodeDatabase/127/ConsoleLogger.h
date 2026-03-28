#ifndef CONSOLELOGGER_H
#define CONSOLELOGGER_H

#include "Logger.h"
#include <string>

// Реализация Logger для записи в консоль
class ConsoleLogger : public Logger {
private:
	std::string getCurrentTime() const;

public:
	void log(const std::string& message) override;
};

#endif 





