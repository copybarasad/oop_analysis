#include "quit_command.h"
#include "game.h"

void QuitCommand::execute(Game& game) {
    game.gameOver = true;
}
