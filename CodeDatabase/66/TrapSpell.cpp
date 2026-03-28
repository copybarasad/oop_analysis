#include "../include/TrapSpell.h"
#include "../include/GameWorld.h"
#include "../include/Trap.h"
#include <cmath>

TrapSpell::TrapSpell(int damage, int range) 
    : baseDamage(30), baseRange(range) {}

bool TrapSpell::canCast(const Position& playerPos, const SpellTarget& target, const GameWorld& world) const {
    int dx = target.targetPosition.getX() - playerPos.getX();
    int dy = target.targetPosition.getY() - playerPos.getY();
    int distance = abs(dx) + abs(dy);
    
    if (distance > baseRange) {
        return false;
    }
    
    if (playerPos == target.targetPosition) {
        return false;
    }
    
    return world.getMapGrid().canMoveTo(target.targetPosition) && 
           !world.hasAliveEnemyAt(target.targetPosition) &&
           !world.hasTrapAt(target.targetPosition) &&
           !world.isPositionOccupiedByAlly(target.targetPosition) &&
           !world.hasBuildingAt(target.targetPosition) &&
           !world.hasTowerAt(target.targetPosition);
}

void TrapSpell::cast(const Position& playerPos, const SpellTarget& target, GameWorld& world, EnhancementBuffer& buffer) {
    int enhancements = buffer.getEnhancementCount(SpellType::TRAP);
    int effectiveDamage = baseDamage + (enhancements * 5);
    
    int dx = target.targetPosition.getX() - playerPos.getX();
    int dy = target.targetPosition.getY() - playerPos.getY();
    int distance = abs(dx) + abs(dy);
    
    if (distance <= baseRange) {
        world.getTrapManager().addTrap(target.targetPosition, std::make_unique<Trap>(effectiveDamage));
    }
}

SpellType TrapSpell::getType() const {
    return SpellType::TRAP;
}

std::string TrapSpell::getName() const {
    return "Trap";
}

std::string TrapSpell::getDescription(const EnhancementBuffer& buffer) const {
    int enhancements = buffer.getEnhancementCount(SpellType::TRAP);
    int effectiveDamage = baseDamage + (enhancements * 5);
    return "Place a trap (" + std::to_string(effectiveDamage) + " dmg, range 2)";
}

int TrapSpell::getBaseDamage() const {
    return baseDamage;
}

int TrapSpell::getBaseRange() const {
    return baseRange;
}
