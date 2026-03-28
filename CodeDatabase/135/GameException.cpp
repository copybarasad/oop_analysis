#include "GameException.h"

GameException::GameException(const std::string& message, const std::string& loc)
	: std::runtime_error(message), location(loc), full_message() {}

const char* GameException::what() const noexcept {
	full_message = "[" + location + "] " + std::runtime_error::what();
	return full_message.c_str();
}

const std::string& GameException::getLocation() const {
	return location;
}

SaveException::SaveException(const std::string& message, const std::string& loc)
	: GameException("Save Error: " + message, loc) {}

FileWriteException::FileWriteException(const std::string& filename, const std::string& loc)
	: SaveException("Cannot write to file '" + filename + "'", loc) {}

LoadException::LoadException(const std::string& message, const std::string& loc)
	: GameException("Load Error: " + message, loc) {}

FileNotFoundException::FileNotFoundException(const std::string& filename, const std::string& loc)
	: LoadException("File '" + filename + "' not found", loc) {}

CorruptedDataException::CorruptedDataException(const std::string& details, const std::string& loc)
	: LoadException("Corrupted data: " + details, loc) {}
