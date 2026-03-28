#include "SpellValidationSystem.h"
#include "Player.h"
#include "Spell.h"
#include "GameField.h"
#include "TargetFinder.h"

bool SpellValidationSystem::canCastSpell(const Player& caster, const Spell& spell, const GameField& field) {
    return caster.getMana() >= spell.getManaCost();
}

bool SpellValidationSystem::hasValidTargets(const Player& caster, const GameField& field, int range) {
    auto targets = TargetFinder::findTargetsInRange(caster, field, range);
    return !targets.empty();
}

bool SpellValidationSystem::hasValidPositions(const Player& caster, const GameField& field, int range) {
    auto positions = TargetFinder::findAvailableTrapPositions(caster, field, range);
    return !positions.empty();
}