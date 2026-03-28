#include "load_selected_save_command.h"

LoadSelectedSaveCommand::LoadSelectedSaveCommand(int index) : saveIndex(index) {}

bool LoadSelectedSaveCommand::execute(GameController& game) {
	(void)game;
	return true;
}

std::string LoadSelectedSaveCommand::getDescription() const {
	return "Load Selected Save";
}

bool LoadSelectedSaveCommand::isValid() const {
	return saveIndex >= 0;
}