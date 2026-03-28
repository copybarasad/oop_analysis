#ifndef CORRUPTED_DATA_EXCEPTION_H
#define CORRUPTED_DATA_EXCEPTION_H

#include "load_game_exception.h"
#include <string>

class CorruptedDataException : public LoadGameException {
public:
	explicit CorruptedDataException(const std::string& context)
		: LoadGameException("Corrupted data detected in " + context) {}
};

#endif





