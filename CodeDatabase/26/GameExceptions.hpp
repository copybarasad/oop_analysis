#pragma once

#include <stdexcept>
#include <string>

class GameException : public std::runtime_error {
public:
	explicit GameException(const std::string& message)
		: std::runtime_error(message) {}
};

class SaveLoadException : public GameException {
public:
	explicit SaveLoadException(const std::string& message)
		: GameException("Save/Load error: " + message) {}
};

class InvalidDataException : public GameException {
public:
	explicit InvalidDataException(const std::string& message)
		: GameException("Invalid data: " + message) {}
};
