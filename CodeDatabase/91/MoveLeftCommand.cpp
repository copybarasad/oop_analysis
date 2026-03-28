#include "MoveLeftCommand.h"
#include "Game.h"

void MoveLeftCommand::execute(Game& game) {
    game.processPlayerTurn('A');
}