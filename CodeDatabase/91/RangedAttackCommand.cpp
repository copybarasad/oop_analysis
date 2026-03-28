#include "RangedAttackCommand.h"
#include "Game.h"

void RangedAttackCommand::execute(Game& game) {
    game.processPlayerTurn('F');

}