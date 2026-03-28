#include "QuitCommand.h"
#include "../Game.h"

CommandResult QuitCommand::execute(Game& game) {
    game.SetGameOver(true);
    return {true, "Quitting..."};
}
