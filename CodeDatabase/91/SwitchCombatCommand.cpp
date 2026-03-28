#include "SwitchCombatCommand.h"
#include "Game.h"

void SwitchCombatCommand::execute(Game& game) {
    game.processPlayerTurn('C');
}