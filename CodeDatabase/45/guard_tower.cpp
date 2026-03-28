#include "guard_tower.hpp"
#include <iostream>

GuardTower::GuardTower(int health, int damage) 
    : Entity(health, damage), 
      spell(), charge(0) {
}

void GuardTower::step(Map* map, int x, int y) {
    if (charge < 3) {
        charge += 1;
    } else {
        int width = map->getWidth();
        int height = map->getHeight();
        bool find_player = false;
        
        for (int dy = -3; dy <= 3; dy++) {
            for (int dx = -3; dx <= 3; dx++) {
                //if (abs(dx) + abs(dy) > 3) continue;
                if (dx == 0 && dy == 0) continue;
                
                int nx = x + dx;
                int ny = y + dy;
                
                if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
                    if (map->getCellState(nx, ny) == CellState::PLAYER) {
                        spell.shoot(map, nx, ny, damage);
                        find_player = true;
                        charge = 0;
                        break;
                    }
                }
            }
            if (find_player) break;
        }
        
        //charge = 0;
    }
}

std::string GuardTower::getStatus() const {
    return "GuardTower," + std::to_string(hp) + "," + std::to_string(damage) + "," + std::to_string(charge);
}

void GuardTower::setCharge(int new_charge) {
    charge = new_charge;
}