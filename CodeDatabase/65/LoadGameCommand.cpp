#include "commands/LoadGameCommand.h"
#include "Game.h"

void LoadGameCommand::execute(Game& game) {
    game.handleLoadGame();
}
