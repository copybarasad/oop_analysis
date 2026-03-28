#include "commands/ChangeAttackModeCommand.h"
#include "Game.h"

void ChangeAttackModeCommand::execute(Game& game) {
    game.handleChangeAttackMode();
}
