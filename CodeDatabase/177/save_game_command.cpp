#include "save_game_command.h"

bool SaveGameCommand::execute(GameController& game) {
	(void)game;
	return true;
}

std::string SaveGameCommand::getDescription() const {
	return "Save Game";
}

bool SaveGameCommand::isValid() const {
	return true;
}

