#include "invalid_command.h"
#include "game.h"
#include <iostream>

void InvalidCommand::execute(Game& game) {
    std::cout << "Invalid command. Try again.\n";
}
