//
// Created by Mac on 02.12.2025.
//

#include "UseSpellCommand.h"

CommandType UseSpellCommand::getType() const {
    return CommandType::USE_SPELL;
}

CommandResult UseSpellCommand::execute(Game &game) {
    auto field = game.getField();
    auto player = field->getPlayer();

    auto spellController = std::make_unique<SpellController>(field);
    spellController->useSpell();

    return CommandResult::success();
}


