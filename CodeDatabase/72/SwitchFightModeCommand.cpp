//
// Created by Mac on 02.12.2025.
//

#include "SwitchFightModeCommand.h"

enum class FightMode;

CommandType SwitchFightModeCommand::getType() const {
    return CommandType::SWITCH_FIGHT_MODE;
}

CommandResult SwitchFightModeCommand::execute(Game &game) {
    auto player = game.getField()->getPlayer();

    FightMode oldMode = player->getFightMode();

    player->switchFightMode();

    FightMode newMode = player->getFightMode();

    std::string message;
    if (oldMode == FightMode::CloseCombat && newMode == FightMode::RangedCombat) {
        message = "Switched to ranged combat mode!";
    } else if (oldMode == FightMode::RangedCombat && newMode == FightMode::CloseCombat) {
        message = "Switched to close combat mode!";
    } else {
        message = "Fight mode switched!";
    }

    return CommandResult::success(message);
}


