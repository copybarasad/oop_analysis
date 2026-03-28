#ifndef GAME_SAVE_EXCEPTION_H
#define GAME_SAVE_EXCEPTION_H

#include <stdexcept>
#include <string>

class GameSaveException : public std::runtime_error {
public:
	explicit GameSaveException(const std::string& message);
};

#endif

