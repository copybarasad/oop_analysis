#include "DirectDamageSpell.h"
#include "GameField.h"

DirectDamageSpell::DirectDamageSpell() 
    : name("Магический выстрел"),
      description("Наносит 15 урона одной цели в радиусе 5 клеток"),
      damage(15), range(5) {}

bool DirectDamageSpell::canCast(const Position& casterPos, const Position& targetPos, const GameField& field) const {
    if (!field.isPositionValid(targetPos)) return false;
    double distance = PositionUtils::calculateDistance(casterPos, targetPos);
    return distance <= range;
}

void DirectDamageSpell::cast(const Position& casterPos, const Position& targetPos, GameField& field) {
    // Логика в CombatSystem
}