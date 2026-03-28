#include "switch_mode_command.h"
#include "game.h"
#include <iostream>

void SwitchModeCommand::execute(Game& game) {
    game.player.switchCombatMode();
    std::cout << "Mode switched to: " << (game.player.getCombatMode() == CombatMode::MELEE ? "MELEE" : "RANGED") << "\n";
}
