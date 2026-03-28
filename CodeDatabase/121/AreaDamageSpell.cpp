#include "AreaDamageSpell.h"
#include "GameField.h"
#include "Enemy.h"
#include "EnemyBuilding.h"

AreaDamageSpell::AreaDamageSpell() 
    : name("Огненный шар"),
      description("Наносит 10 урона по области 2x2 клетки"),
      range(4) {}

bool AreaDamageSpell::canCast(const Position& casterPos, const Position& targetPos, const GameField& field) const {
    if (!field.isPositionValid(targetPos)) return false;
    double distance = PositionUtils::calculateDistance(casterPos, targetPos);
    return distance <= range;
}

void AreaDamageSpell::cast(const Position& casterPos, const Position& targetPos, GameField& field) {
    // Логика будет в CombatSystem
    // Подавляем предупреждения о неиспользуемых параметрах
    (void)casterPos;
    (void)targetPos;
    (void)field;
}