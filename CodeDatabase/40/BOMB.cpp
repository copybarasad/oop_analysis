#include "BOMB.h"
#include "Game.h"
#include <iostream>

BOMB::BOMB(int id, const std::string& name, int cost, int damage, int radius)
    : Spell(id, name, cost), damage(damage), radius(radius) {}

void BOMB::apply(Character& caster, Character& target) const {
    std::cout << "this spell is area-based! Use coordinates.\n";
}

void BOMB::applyArea(Character& caster, int centerX, int centerY, Game& game) {
    std::cout << caster.getName() << " casts " << name
              << " at (" << centerX << ", " << centerY << ")!\n";
    game.damageEnemyAt(centerX, centerY, damage, radius);
}