#include "MoveDownCommand.h"
#include "Game.h"

void MoveDownCommand::execute(Game& game) {
    game.processPlayerTurn('S');
}