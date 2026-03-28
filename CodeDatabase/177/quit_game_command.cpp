#include "quit_game_command.h"

bool QuitGameCommand::execute(GameController& game) {
	(void)game;
	return false;
}

std::string QuitGameCommand::getDescription() const {
	return "Quit game";
}

bool QuitGameCommand::isValid() const {
	return true;
}





