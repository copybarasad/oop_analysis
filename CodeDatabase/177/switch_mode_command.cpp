#include "switch_mode_command.h"

bool SwitchModeCommand::execute(GameController& game) {
	game.switchPlayerCombatMode();
	return true;
}

std::string SwitchModeCommand::getDescription() const {
	return "Switch combat mode";
}

bool SwitchModeCommand::isValid() const {
	return true;
}





