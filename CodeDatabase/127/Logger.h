#ifndef LOGGER_H
#define LOGGER_H

#include <string>

// Интерфейс для логирования
class Logger {
public:
	virtual ~Logger() = default;
	virtual void log(const std::string& message) = 0;
};

#endif 





