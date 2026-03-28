#include "../include/EnhancementSpell.h"
#include "../include/GameWorld.h"

EnhancementSpell::EnhancementSpell() {}

bool EnhancementSpell::canCast(const Position& playerPos, const SpellTarget& target, const GameWorld& world) const {
    return true;
}

void EnhancementSpell::cast(const Position& playerPos, const SpellTarget& target, GameWorld& world, EnhancementBuffer& buffer) {
    buffer.addEnhancement();
}

SpellType EnhancementSpell::getType() const {
    return SpellType::ENHANCEMENT;
}

std::string EnhancementSpell::getName() const {
    return "Empower";
}

std::string EnhancementSpell::getDescription(const EnhancementBuffer& buffer) const {
    return "Enhance all spells permanently (+1 to all types)";
}
