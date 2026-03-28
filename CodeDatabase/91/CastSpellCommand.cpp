#include "CastSpellCommand.h"
#include "Game.h"

void CastSpellCommand::execute(Game& game) {
    game.processPlayerTurn('M');
}