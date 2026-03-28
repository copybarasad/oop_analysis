#include "tower_spell.h"
#include <iostream>
#include "constants.h"

TowerSpell::TowerSpell() 
    : name("Башенный луч"), cost(constants::TOWER_SPELL_COST), 
      range(constants::TOWER_SPELL_RADIUS), power(constants::TOWER_SPELL_POWER) {}


bool TowerSpell::canCast(int targetX, int targetY, int towerX, int towerY, Field& field) const {
    int distance = std::max(std::abs(towerX - targetX), std::abs(towerY - targetY));
    if (distance > range) {
        return false;
    }

    if (!field.isValidPosition(targetX, targetY)) {
        return false;
    }

    return true;
}

int TowerSpell::cast_spell(int targetX, int targetY, int towerX, int towerY, Field& field) {
    if (!canCast(targetX, targetY, towerX, towerY, field)) {
        return 0;
    }

    Cell& targetCell = field.getCell(targetX, targetY);
    
    if (targetCell.getType() != Type::PLAYER) {
        return 0;
    }

    std::cout << "Башня использует " << name << " на игрока в (" 
              << targetX << ", " << targetY << ")! Нанесено " << power << " урона." << std::endl;
    return power;
}
