#include "gamefield.h"
#include "player.h"
#include "enemy.h" 
#include <iostream>
#include <memory>   
 
GameField::GameField(int w, int h) {
    map = std::make_unique<MapManager>(w, h);            
    objectPlacer = std::make_unique<ObjectPlacer>(*map);  
}
 
void GameField::reset(int w, int h) { 
    traps.clear();
    towers.clear();
     
    map = std::make_unique<MapManager>(w, h);
    objectPlacer = std::make_unique<ObjectPlacer>(*map);
}
 


bool GameField::placeTrap(int x, int y, int damage) {
    if (!isValidPosition(x, y)) {   
        return false;
    }
    
    Cell& cell = getCell(x, y);     
    if (!cell.getIsPassable() || cell.getIsOccupied()) {
        return false;
    }
    
    Trap newTrap(damage);
    newTrap.place(x, y);
    traps.push_back(newTrap);
    
    return true;
}

bool GameField::hasTrapAt(int x, int y) const {
    for (const auto& trap : traps) {
        int trapX, trapY;
        trap.getPosition(trapX, trapY);
        if (trapX == x && trapY == y && trap.getIsActive()) {
            return true;
        }
    }
    return false;
}

void GameField::updateTraps(Enemy& enemy) {
    if (!enemy.isAlive()) return;
    
    int enemyX, enemyY;
    if (!findEnemyPosition(enemyX, enemyY)) return;   
    
    for (auto it = traps.begin(); it != traps.end(); ) {
        if (it->checkTrigger(enemyX, enemyY, enemy)) {
            it = traps.erase(it);
        } else {
            it->update();
            if (!it->getIsActive()) {
                it = traps.erase(it);
            } else {
                ++it;
            }
        }
    }
}

bool GameField::placeTower(int x, int y) {
    if (!isValidPosition(x, y)) {  
        return false;
    }
    
    Cell& cell = getCell(x, y);    
    if (!cell.getIsPassable() || cell.getIsOccupied()) {
        return false;
    }
    
    EnemyTower newTower;
    newTower.place(x, y);
    towers.push_back(newTower); 
    cell.setIsPassable(false);
    
    return true;
}

void GameField::updateTowers(Player& player) {
    for (auto it = towers.begin(); it != towers.end(); ) {
        it->update(*this, player);  
        
        if (!it->getIsAlive()) { 
            int x, y;
            it->getPosition(x, y);
            if (isValidPosition(x, y)) {   
                getCell(x, y).setIsPassable(true);  
            }
            it = towers.erase(it);
        } else {
            ++it;
        }
    }
}

std::vector<EnemyTower>& GameField::getTowers() {
    return towers;
}

const std::vector<EnemyTower>& GameField::getTowers() const {
    return towers;
}

bool GameField::hasTowerAt(int x, int y) const {
    for (const auto& tower : towers) {
        int towerX, towerY;
        tower.getPosition(towerX, towerY);
        if (towerX == x && towerY == y && tower.getIsAlive()) {
            return true;
        }
    }
    return false;
}

void GameField::displayTowersInfo() const {
    if (!towers.empty()) {
        std::cout << "--- Вражеские башни ---\n";
        for (const auto& tower : towers) {
            if (tower.getIsAlive()) {
                int x, y;
                tower.getPosition(x, y);
                std::cout << "Башня в (" << x << "," << y << "): " 
                          << tower.getHealth() << "/75 HP\n";
            }
        }
    }
}