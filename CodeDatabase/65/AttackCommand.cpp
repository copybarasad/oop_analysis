#include "commands/AttackCommand.h"
#include "Game.h"

void AttackCommand::execute(Game& game) {
    game.handleAttack();
}
