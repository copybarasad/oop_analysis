#include "switch_mode_command.h"
#include "game_session.h"

void SwitchModeCommand::Execute(GameSession& game) {
    game.ProcessSwitchMode();
}
