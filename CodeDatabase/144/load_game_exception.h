#ifndef LOAD_GAME_EXCEPTION_H
#define LOAD_GAME_EXCEPTION_H

#include <stdexcept>
#include <string>

class LoadGameException : public std::runtime_error {
public:
	explicit LoadGameException(const std::string& message)
		: std::runtime_error("LoadGameException: " + message) {}
};

#endif





