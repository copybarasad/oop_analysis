#ifndef GAME_LOAD_EXCEPTION_H
#define GAME_LOAD_EXCEPTION_H

#include <stdexcept>
#include <string>

class GameLoadException : public std::runtime_error {
public:
	explicit GameLoadException(const std::string& message);
};

#endif

