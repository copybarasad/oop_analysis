#include "SwitchCombatCommand.h"
#include "../Game.h"

namespace Game {

void SwitchCombatCommand::execute(Game& game) {
    game.playerSwitchCombatMode();
}

}
