#include "area_damage_spell.h"
#include <iostream>
#include <vector>

AreaDamageSpell::AreaDamageSpell() 
    : SpellBasis("Огненный шар", constants::AREA_SPELL_COST, constants::AREA_SPELL_RADIUS, constants::AREA_SPELL_POWER) {}

bool AreaDamageSpell::canCast(int casterX, int casterY, int targetX, int targetY, int enemyX, int enemyY, const Field& field) const {
        if ((enemyX < targetX) || (enemyX > targetX + 2) || (enemyY < targetY) || (enemyY > targetY + 2)) { return false; }
        
        int distance = std::max(std::abs(targetX - casterX), std::abs(targetY - casterY));
        if (distance > range) {
            std::cout << "Цель вне радиуса заклинания! Радиус: " << range << ", расстояние: " << distance << std::endl;
            return false;
        }

        if (!field.isValidPosition(targetX, targetY)) {
            std::cout << "Недопустимая цель!" << std::endl;
            return false;
        }

        return true;
}

int AreaDamageSpell::cast_spell(int targetX, int targetY, int enemyX, int enemyY, Field& field, Player& player) {
    if (!canCast(player.getX(), player.getY(), targetX, targetY, enemyX, enemyY, field)) {
        return 0;
    }

    std::cout << player.getname() << " использует " << name << " на область вокруг (" 
              << targetX << ", " << targetY << ")" << std::endl;
              
    return power;
}
