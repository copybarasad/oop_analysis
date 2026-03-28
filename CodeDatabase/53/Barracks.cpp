#include "Barracks.h"
#include "GameField.h"
#include "Enemy.h"
#include "Names.h"
#include "Constants.h"
#include <iostream>
#include <cstdlib>

using namespace GameConstants;

Barracks::Barracks(int x, int y, int cooldown)
    : Building(x, y, BARRACKS_HEALTH, true), spawnCooldown(cooldown), currentCooldown(cooldown) {
    if (spawnCooldown < 1) spawnCooldown = 1;
}

void Barracks::update(GameField& field) {
    
    if (currentCooldown > 0) {
        currentCooldown--;
        return;
    }
    
    int directions[4][2] = {{1,0}, {-1,0}, {0,1}, {0,-1}};
    
    for (int i = 0; i < 4; i++) {
        int newX = getX() + directions[i][0];
        int newY = getY() + directions[i][1];
        
        if (field.canMoveTo(newX, newY) && field.getEnemyCount() < field.getMaxCntEnemy()) {
            try {
                std::string enemyName = CowboyNames::names_arr[std::rand() % CowboyNames::NAME_COUNT];
                auto newEnemy = std::make_unique<Enemy>(enemyName, BARRACKS_ENEMY_HEALTH, BARRACKS_ENEMY_DAMAGE, newX, newY);
                
                field.arr_enemies.push_back(std::move(newEnemy));
                field.setCellType(newX, newY, ENEMY);
                
                currentCooldown = spawnCooldown;

                return;
            } catch (const std::exception& e) {
                std::cout << "Ошибка создания врага: " << e.what() << std::endl;
            }
        }
    }
    currentCooldown = RETRY_COOLDOWN;   
}


int Barracks::getCooldown() const { return spawnCooldown; }
int Barracks::getCurrentCooldown() const { return currentCooldown; }



void Barracks::setCooldown(int cooldown) { 
    spawnCooldown = cooldown; 
    if (currentCooldown > spawnCooldown) {
        currentCooldown = spawnCooldown;
    }
}
void Barracks::setCurrentCooldown(int cooldown){currentCooldown = cooldown;}