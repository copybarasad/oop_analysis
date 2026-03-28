//
// Created by Mac on 02.12.2025.
//

#include "ShootCommand.h"

ShootCommand::ShootCommand(const Direction &dir)
    :direction(dir) {}

CommandType ShootCommand::getType() const {
    return CommandType::SHOOT;
}

CommandResult ShootCommand::execute(Game &game) {
    auto player = game.getField()->getPlayer();

    if (player->getFightMode() != FightMode::RangedCombat) {
        return CommandResult::failure("You need to switch to ranged combat mode first!");
    }

    auto field = game.getField();
    auto combat = std::make_unique<CombatController>(field);
    combat->playerShoot(direction);
    return CommandResult::success();
}




