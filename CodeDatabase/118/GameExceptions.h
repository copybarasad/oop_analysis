#ifndef EXCEPTIONS
#define EXCEPTIONS

#include <stdexcept>
#include <string>

class GameException : public std::runtime_error
{
public:
	explicit GameException(const std::string& message)
		: std::runtime_error(message) {}
};

class InputException : public GameException
{
public:
	InputException(const std::string& message)
		: GameException("Input Exception: " + message) {}
};

class CommandException : public GameException
{
public:
	CommandException(const std::string& message)
		: GameException("Command Exception: " + message) {}
};

class MovementException : public GameException
{
public:
	MovementException(const std::string& message)
		: GameException("Movement Exception: " + message) {
	}
};

class AttackException : public GameException
{
public:
	AttackException(const std::string& message)
		: GameException("Attack Exception: " + message) {
	}
};

class SpellException : public GameException
{
public:
	SpellException(const std::string& message)
		: GameException("Spell Exception: " + message) {
	}
};

class FileException : public GameException
{
public:
	FileException(const std::string& message)
		: GameException("File Exception: " + message) {
	}
};
#endif