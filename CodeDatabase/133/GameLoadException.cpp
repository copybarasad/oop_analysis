#include "GameLoadException.h"

GameLoadException::GameLoadException(const std::string& message)
	: std::runtime_error(message) {}

