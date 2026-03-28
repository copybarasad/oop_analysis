//
// Created by Mac on 02.12.2025.
//

#include "BuySpellCommand.h"

#include <iostream>

#define CLEAR_COMMAND "clear"

CommandType BuySpellCommand::getType() const {
    return CommandType::BUY_SPELL;
}


CommandResult BuySpellCommand::execute(Game &game) {
    auto player = game.getField()->getPlayer();
    auto shop = game.getSpellShop();

    system(CLEAR_COMMAND);

    std::cout << "\nYou have " << player->getScore() << " points.\n";

    shop.buySpell(*player);

    return CommandResult::success();
}

