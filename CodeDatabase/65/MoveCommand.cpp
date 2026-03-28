#include "commands/MoveCommand.h"
#include "Game.h"

void MoveCommand::execute(Game& game) {
    game.handleMove(direction);
}
