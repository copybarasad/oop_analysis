#include "open_load_menu_command.h"

bool OpenLoadMenuCommand::execute(GameController& game) {
	(void)game;
	return true;
}

std::string OpenLoadMenuCommand::getDescription() const {
	return "Open Load Menu";
}

bool OpenLoadMenuCommand::isValid() const {
	return true;
}

