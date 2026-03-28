#include "start_new_game_command.h"

bool StartNewGameCommand::execute(GameController& game) {
	(void)game; 
	return true;
}

std::string StartNewGameCommand::getDescription() const {
	return "Start New Game";
}

bool StartNewGameCommand::isValid() const {
	return true;
}

