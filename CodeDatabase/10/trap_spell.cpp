#include "trap_spell.h"
#include <iostream>
#include "constants.h"

TrapSpell::TrapSpell() 
    : SpellBasis("Магическая ловушка", constants::TRAP_SPELL_COST, constants::TRAP_SPELL_RADIUS, constants::TRAP_SPELL_POWER) {}

bool TrapSpell::canCast(int casterX, int casterY, int targetX, int targetY, const Field& field) const {
    int distance = std::max(std::abs(targetX - casterX), std::abs(targetY - casterY));
    if (distance > range) {
        std::cout << "Цель вне радиуса заклинания! Радиус: " << range << ", расстояние: " << distance << std::endl;
        return false;
    }

    if (!field.isValidPosition(targetX, targetY)) {
        std::cout << "Недопустимая цель!" << std::endl;
        return false;
    }

    Cell& targetCell = const_cast<Field&>(field).getCell(targetX, targetY);
    if (!targetCell.isEmpty()) {
        std::cout << "Клетка должна быть пустой для установки ловушки!" << std::endl;
        return false;
    }

    return true;
}

int TrapSpell::cast_spell(int targetX, int targetY, int, int, Field& field, Player& player) {
    if (!canCast(player.getX(), player.getY(), targetX, targetY, field)) {
        return 0;
    }

    Cell& targetCell = field.getCell(targetX, targetY);
    targetCell.setDamageTrap();
    
    std::cout << player.getname() << " устанавливает " << name << " в позиции (" 
              << targetX << ", " << targetY << ")" << std::endl;
              
    return 0;
}
