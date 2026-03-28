#ifndef GAME_EXCEPTIONS_H
#define GAME_EXCEPTIONS_H

#include <stdexcept>
#include <string>

// Base exception class for all game-related exceptions
class GameException : public std::runtime_error {
public:
	explicit GameException(const std::string& message) 
		: std::runtime_error("Game Error: " + message) {}
};

// Exception for file save/load operations
class SaveLoadException : public GameException {
public:
	explicit SaveLoadException(const std::string& operation, const std::string& filename, const std::string& reason)
		: GameException("Failed to " + operation + " file '" + filename + "': " + reason) {}
};

// Exception for corrupted save file data
class CorruptedSaveException : public GameException {
public:
	explicit CorruptedSaveException(const std::string& filename, const std::string& details)
		: GameException("Corrupted save file '" + filename + "': " + details) {}
};

// Exception for invalid game state operations
class InvalidGameStateException : public GameException {
public:
	explicit InvalidGameStateException(const std::string& operation, const std::string& currentState)
		: GameException("Cannot perform '" + operation + "' in current state: " + currentState) {}
};

#endif // GAME_EXCEPTIONS_H