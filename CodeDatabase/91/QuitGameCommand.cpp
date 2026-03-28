#include "QuitGameCommand.h"
#include "Game.h"

void QuitGameCommand::execute(Game& game) {
    game.processPlayerTurn('Q');

}