#include "save_command.h"
#include "game_session.h"

void SaveCommand::Execute(GameSession& game) {
    game.ProcessSaveGame();
}
