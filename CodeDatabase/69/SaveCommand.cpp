#include "SaveCommand.h"
#include "../Game.h"
#include <iostream>

SaveCommand::SaveCommand(const std::string& filename) : filename_(filename) {}

CommandResult SaveCommand::execute(Game& game) {
    try {
        game.Save(filename_);
        return {true, "Saved to " + filename_};
    } catch (const std::exception& e) {
        return {false, "Save failed: " + std::string(e.what())};
    }
}


