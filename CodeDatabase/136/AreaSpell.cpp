#include "AreaSpell.h"
#include <iostream>

AreaSpell::AreaSpell(std::string name, int damage, int manaCost, int range)
    : name(name), damage(damage), manaCost(manaCost), range(range) {
    description = "Deals " + std::to_string(damage) + " damage in 2x2 area";
}

void AreaSpell::cast(Position target, GameField& field) {
    int hitCount = 0;
    
    for (int dx = 0; dx < 2; ++dx) {
        for (int dy = 0; dy < 2; ++dy) {
            int x = target.x + dx;
            int y = target.y + dy;
            
            if (field.isValidPosition(x, y)) {
                Cell& cell = field.getCell(x, y);
                
                if (cell.getHasBeast()) {
                    std::cout << name << " hits beast at (" << x << "," << y << ") for " << damage << " damage!\n";
                    hitCount++;
                }
                
                if (cell.getHasBuilding()) {
                    std::cout << name << " hits building at (" << x << "," << y << ") for " << damage << " damage!\n";
                    hitCount++;
                }
            }
        }
    }
    
    if (hitCount == 0) {
        std::cout << name << " explodes but hits nothing!\n";
    } else {
        std::cout << name << " hit " << hitCount << " targets!\n";
    }
}