#include "commands/QuitCommand.h"
#include "Game.h"

void QuitCommand::execute(Game& game) {
    game.handleQuit();
}
