#include "../include/AreaDamageSpell.h"
#include "../include/GameWorld.h"
#include <cmath>

AreaDamageSpell::AreaDamageSpell(int damage, int range, int areaSize) 
    : baseDamage(damage), baseRange(range), baseAreaSize(areaSize) {}

bool AreaDamageSpell::canCast(const Position& playerPos, const SpellTarget& target, const GameWorld& world) const {
    int dx = target.targetPosition.getX() - playerPos.getX();
    int dy = target.targetPosition.getY() - playerPos.getY();
    int distance = abs(dx) + abs(dy);
    
    return distance <= baseRange;
}

void AreaDamageSpell::cast(const Position& playerPos, const SpellTarget& target, GameWorld& world, EnhancementBuffer& buffer) {
    int enhancements = buffer.getEnhancementCount(SpellType::AREA_DAMAGE);
    int effectiveAreaSize = baseAreaSize + enhancements;
    
    int dx = target.targetPosition.getX() - playerPos.getX();
    int dy = target.targetPosition.getY() - playerPos.getY();
    int distance = abs(dx) + abs(dy);
    
    if (distance > baseRange) {
        return;
    }
    
    for (int y = 0; y < effectiveAreaSize; y++) {
        for (int x = 0; x < effectiveAreaSize; x++) {
            Position damagePos(target.targetPosition.getX() + x, target.targetPosition.getY() + y);
            if (world.hasAliveEnemyAt(damagePos)) {
                world.getEnemyManager().attackEnemyAt(damagePos, baseDamage);
            }
        }
    }
}

SpellType AreaDamageSpell::getType() const {
    return SpellType::AREA_DAMAGE;
}

std::string AreaDamageSpell::getName() const {
    return "Meteor";
}

std::string AreaDamageSpell::getDescription(const EnhancementBuffer& buffer) const {
    int enhancements = buffer.getEnhancementCount(SpellType::AREA_DAMAGE);
    int effectiveArea = baseAreaSize + enhancements;
    return "Area damage spell (15 dmg, " + std::to_string(effectiveArea) + "x" + std::to_string(effectiveArea) + " area, range 4)";
}

int AreaDamageSpell::getBaseDamage() const {
    return baseDamage;
}

int AreaDamageSpell::getBaseRange() const {
    return baseRange;
}

int AreaDamageSpell::getBaseAreaSize() const {
    return baseAreaSize;
}
