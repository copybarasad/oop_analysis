#include "SkipTurnCommand.h"
#include "../Game.h"
#include <iostream>

CommandResult SkipTurnCommand::execute(Game& game) {
    return {true, "Turn skipped."};
}
