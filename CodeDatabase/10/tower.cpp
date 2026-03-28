#include "tower.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "constants.h"

Tower::Tower(std::string name, Field& field, int startX, int startY) 
    : name(name), x(startX), y(startY), field(field) {
        field.getCell(x, y).setTower();
    }

bool Tower::spawnEnemy(std::vector<Enemy*>& enemies) {    
    
    int directions[4][2] = {{1,0}, {0,1}, {-1,0}, {0,-1}};
    
    for (int i = 0; i < 4; i++) {
        int newX = x + directions[i][0];
        int newY = y + directions[i][1];
        
        if (field.isValidPosition(newX, newY) && field.getCell(newX, newY).isEmpty()) {
            Enemy* newEnemy = new Enemy("Страж", field, newX, newY);
            enemies.push_back(newEnemy);
            std :: cout << name << " создала нового врага в позиции (" << newX << ", " << newY << ")\n";
            return true;
        }
    }
    
    return false;
}

std::string Tower::getName() const {
    return name;
}

int Tower::getX() const {
    return x;
}

int Tower::getY() const {
    return y;
}
