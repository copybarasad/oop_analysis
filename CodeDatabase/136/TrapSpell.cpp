#include "TrapSpell.h"
#include <iostream>

TrapSpell::TrapSpell(std::string name, int damage, int manaCost, int range)
    : name(name), damage(damage), manaCost(manaCost), range(range) {
    description = "Places a trap that deals " + std::to_string(damage) + " damage when triggered";
}

void TrapSpell::cast(Position target, GameField& field) {
    if (!field.isValidPosition(target.x, target.y)) {
        std::cout << "Invalid target position for trap!\n";
        return;
    }
    
    Cell& cell = field.getCell(target.x, target.y);
    if (!cell.isEmpty()) {
        std::cout << "Cannot place trap on occupied cell!\n";
        return;
    }
    
    cell.setHasTrap(true);
    cell.setTrapDamage(damage);
    std::cout << name << " placed at position (" << target.x << ", " << target.y << ")\n";
}