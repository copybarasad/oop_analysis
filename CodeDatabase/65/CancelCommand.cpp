#include "commands/CancelCommand.h"
#include "Game.h"

void CancelCommand::execute(Game& game) {
    game.handleCancel();
}
