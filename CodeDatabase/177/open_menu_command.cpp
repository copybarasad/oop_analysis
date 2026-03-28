#include "open_menu_command.h"

bool OpenMenuCommand::execute(GameController& game) {
	(void)game;
	return true;
}

std::string OpenMenuCommand::getDescription() const {
	return "Open game menu";
}

bool OpenMenuCommand::isValid() const {
	return true;
}





