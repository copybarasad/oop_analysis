#include "DirectDamageSpell.h"
#include <iostream>

DirectDamageSpell::DirectDamageSpell(std::string name, int damage, int manaCost, int range)
    : name(name), damage(damage), manaCost(manaCost), range(range) {
    description = "Deals " + std::to_string(damage) + " damage to single target";
}

void DirectDamageSpell::cast(Position target, GameField& field) {
    if (!field.isValidPosition(target.x, target.y)) {
        std::cout << "Invalid target position!\n";
        return;
    }
    
    Cell& cell = field.getCell(target.x, target.y);
    bool hitSomething = false;
    
    if (cell.getHasBeast()) {
        std::cout << name << " hits beast for " << damage << " damage!\n";
        hitSomething = true;
    }
    
    if (cell.getHasBuilding()) {
        std::cout << name << " hits building for " << damage << " damage!\n";
        hitSomething = true;
    }
    
    if (!hitSomething) {
        std::cout << name << " fizzles no valid target!\n";
    }
}