#ifndef INVALID_DATA_EXCEPTION_H
#define INVALID_DATA_EXCEPTION_H

#include "load_game_exception.h"
#include <string>

class InvalidDataException : public LoadGameException {
public:
	InvalidDataException(const std::string& context, const std::string& details)
		: LoadGameException("Invalid data in " + context + ": " + details) {}
};

#endif





