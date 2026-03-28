#include "BuySpellCommand.h"
#include "Game.h"

void BuySpellCommand::execute(Game& game) {
    game.processPlayerTurn('B');
}