#include "../include/DirectDamageSpell.h"
#include "../include/GameWorld.h"
#include <cmath>

DirectDamageSpell::DirectDamageSpell(int damage, int range) 
    : baseDamage(damage), baseRange(range) {}

bool DirectDamageSpell::canCast(const Position& playerPos, const SpellTarget& target, const GameWorld& world) const {
    return world.hasAliveEnemyAt(target.targetPosition) || world.hasBuildingAt(target.targetPosition) || world.hasTowerAt(target.targetPosition);
}

void DirectDamageSpell::cast(const Position& playerPos, const SpellTarget& target, GameWorld& world, EnhancementBuffer& buffer) {
    world.attackTargetAt(target.targetPosition, baseDamage);
}

SpellType DirectDamageSpell::getType() const {
    return SpellType::DIRECT_DAMAGE;
}

std::string DirectDamageSpell::getName() const {
    return "Fireball";
}

std::string DirectDamageSpell::getDescription(const EnhancementBuffer& buffer) const {
    int enhancements = buffer.getEnhancementCount(SpellType::DIRECT_DAMAGE);
    int effectiveRange = baseRange + enhancements;
    return "Direct damage spell (30 dmg, range " + std::to_string(effectiveRange) + ")";
}

int DirectDamageSpell::getBaseDamage() const {
    return baseDamage;
}

int DirectDamageSpell::getBaseRange() const {
    return baseRange;
}
