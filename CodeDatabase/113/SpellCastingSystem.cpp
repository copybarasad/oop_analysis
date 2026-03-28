#include "SpellCastingSystem.h"
#include "Spell.h"
#include "Player.h"
#include <iostream>

bool SpellCastingSystem::castSpellWithManaCheck(Player& caster, GameField& field, Spell& spell) {
    if (!validateSpellCastConditions(caster, spell)) {
        return false;
    }

    std::cout << "Casting " << spell.getName() << "..." << std::endl;
    bool result = spell.cast(caster, field);
    handleSpellCastResult(result, spell.getName());

    return result;
}

void SpellCastingSystem::handleSpellCastResult(bool result, const std::string& spellName) {
    if (result) {
        std::cout << "Successfully cast " << spellName << "!" << std::endl;
    }
    else {
        std::cout << spellName << " failed to cast!" << std::endl;
    }
}

bool SpellCastingSystem::validateSpellCastConditions(const Player& caster, const Spell& spell) {
    if (caster.getMana() < spell.getManaCost()) {
        std::cout << "Not enough mana to cast " << spell.getName() << "! Need "
            << spell.getManaCost() << " but have " << caster.getMana() << std::endl;
        return false;
    }
    return true;
}