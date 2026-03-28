#include "LoadCommand.h"
#include "../Game.h"
#include <iostream>

LoadCommand::LoadCommand(const std::string& filename) : filename_(filename) {}

CommandResult LoadCommand::execute(Game& game) {
    try {
        game.Load(filename_);
        return {true, "Loaded " + filename_};
    } catch (const std::exception& e) {
        return {false, "Load failed: " + std::string(e.what())};
    }
}


