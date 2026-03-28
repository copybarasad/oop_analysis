#include "direct_damage_spell.h"
#include <iostream>

DirectDamageSpell::DirectDamageSpell() 
    : SpellBasis("Магическая стрела", constants::DIRECT_SPELL_COST, constants::DIRECT_SPELL_RADIUS, constants::DIRECT_SPELL_POWER) {}


bool DirectDamageSpell::canCast(int casterX, int casterY, int targetX, int targetY, int enemyX, int enemyY, Field& field) const {
        if ((targetX != enemyX) || (targetY != enemyY)) {
            std::cout << "Цель не является врагом или вражеским зданием!" << std::endl;
            return false;
        }
        
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

int DirectDamageSpell::cast_spell(int targetX, int targetY, int enemyX, int enemyY, Field& field, Player& player) {
    if (!canCast(player.getX(), player.getY(), targetX, targetY, enemyX, enemyY, field)) {
        return 0;
    }
    Cell& targetCell = field.getCell(targetX, targetY);
    if (targetCell.hasEnemy()) {
        std::cout << player.getname() << " использует " << name << " на врага в (" 
                  << targetX << ", " << targetY << ")! Нанесено " << power << " урона." << std::endl;
    }
    else if (targetCell.getType() == Type::DAMAGE_TOWER) {
        std::cout << player.getname() << " использует " << name << " на башню в (" 
                  << targetX << ", " << targetY << ")! Нанесено " << power << " урона." << std::endl;
    }

    return power;
}

