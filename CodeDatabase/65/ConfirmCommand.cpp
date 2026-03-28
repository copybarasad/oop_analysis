#include "commands/ConfirmCommand.h"
#include "Game.h"

void ConfirmCommand::execute(Game& game) {
    game.handleConfirm();
}
