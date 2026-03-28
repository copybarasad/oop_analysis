#include "GameSaveException.h"

GameSaveException::GameSaveException(const std::string& message)
	: std::runtime_error(message) {}

