#include "SpellCostSystem.h"
#include "Player.h"
#include "Spell.h"

bool SpellCostSystem::canAffordSpell(const Player& caster, const Spell& spell) {
    return caster.getMana() >= spell.getManaCost();
}

void SpellCostSystem::applySpellCost(Player& caster, const Spell& spell) {
    caster.spendMana(spell.getManaCost());
}

int SpellCostSystem::calculateEffectiveCost(const Player& caster, const Spell& spell) {
    return spell.getManaCost();
}