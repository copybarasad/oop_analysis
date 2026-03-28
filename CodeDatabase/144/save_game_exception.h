#ifndef SAVE_GAME_EXCEPTION_H
#define SAVE_GAME_EXCEPTION_H

#include <stdexcept>
#include <string>

class SaveGameException : public std::runtime_error {
public:
	explicit SaveGameException(const std::string& message)
		: std::runtime_error("SaveGameException: " + message) {}
};

#endif





