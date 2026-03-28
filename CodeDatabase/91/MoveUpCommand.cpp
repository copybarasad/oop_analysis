#include "MoveUpCommand.h"
#include "Game.h"

void MoveUpCommand::execute(Game& game) {
    game.processPlayerTurn('W');
}