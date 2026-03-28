#include "invalid_command.h"

InvalidCommand::InvalidCommand(const std::string& error) : errorMessage(error) {}

bool InvalidCommand::execute(GameController& game) {
	(void)game;
	return true;
}

std::string InvalidCommand::getDescription() const {
	return "Invalid command";
}

bool InvalidCommand::isValid() const {
	return false;
}

std::string InvalidCommand::getErrorMessage() const {
	return errorMessage;
}