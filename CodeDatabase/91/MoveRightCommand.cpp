#include "MoveRightCommand.h"
#include "Game.h"

void MoveRightCommand::execute(Game& game) {
    game.processPlayerTurn('D');
}