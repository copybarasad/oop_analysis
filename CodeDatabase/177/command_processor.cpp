#include "command_processor.h"
#include <iostream>

CommandProcessor::CommandProcessor(GameController* gameController)
	: game(gameController) {}

bool CommandProcessor::processCommand(CommandPtr command) {
	if (!command) {
		return true; // Продолжить игру, но команда невалидна
	}
	
	if (!command->isValid()) {
		std::string error = command->getErrorMessage();
		if (!error.empty()) {
			std::cout << "Error: " << error << std::endl;
		}
		return true; // Продолжить игру, но показать ошибку
	}
	
	if (!game) {
		std::cerr << "Error: Game controller is not set" << std::endl;
		return true;
	}
	
	// Выполняем команду
	return command->execute(*game);
}

bool CommandProcessor::executeCommand(CommandPtr command) {
	if (!command || !command->isValid() || !game) {
		return false;
	}
	
	try {
		command->execute(*game);
		return true;
	} catch (const std::exception& e) {
		std::cerr << "Error executing command: " << e.what() << std::endl;
		return false;
	}
}
