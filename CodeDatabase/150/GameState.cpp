#include "GameState.hpp"
#include <random>
#include <stdexcept>

GameState::GameState(int level) : currentLevel(level), score(0), field(), player(0, 0) {
    resetForNewLevel(level);
}

void GameState::resetForNewLevel(int level) {
    currentLevel = level;
    
    int baseSize = 10 + level * 2;
    if (baseSize > 25) baseSize = 25;
    
    try {
        field = Field(baseSize, baseSize, baseSize/2, baseSize/2);
    } catch (const std::exception& e) {
        field = Field(15, 15, 7, 7);
    }

    int savedScore = player.getScore();
    int savedHealth = player.getMaxHealth();
    int savedMana = player.getMaxMana();
    int savedDamage = player.getDamage();
    
    int heroHealth = savedHealth;
    int heroDamage = savedDamage + 3;
    int startX = field.getWidth() / 2;
    int startY = field.getHeight() / 2;
    
    player.setPosition(startX, startY);
    
    player.setMaxHealth(heroHealth);
    player.setHealth(heroHealth);
    player.setMeleeDamage(heroDamage);
    player.setRangedDamage(heroDamage/2);
    player.setScore(savedScore);
    player.setMana(savedMana);

    player.removeSlowness();
    
    enemies.clear();
    allies.clear();
    buildings.clear();
    traps.clear();
    
    createLevel(level);
}
    

void GameState::createLevel(int level){
    int enemyCount = 3 + level * 2;
    if (enemyCount > 15) enemyCount = 15;    
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> posDist(0, field.getWidth() - 1);
    
    for (int i = 0; i < enemyCount; ++i) {
        int x, y;
        int attempts = 0;
        do {
            x = posDist(gen);
            y = posDist(gen);
            attempts++;
            if (attempts > 100) break;
        } while (!field.isValidPosition(x, y) || 
                 !field.getCell(x, y).isPassable() || 
                 !field.getCell(x, y).isEmpty() ||
                 std::abs(x - player.getPositionX()) + std::abs(y - player.getPositionY()) < 3);
        
        if (attempts <= 100) {
            int enemyHealth = 15 + level * 5;
            int enemyDamage = level * 5;
            enemies.emplace_back(x, y, enemyHealth, enemyDamage);
            field.getCell(x, y).setHasEnemy(true);
        }
    }

    if (level >= 2) {
        int buildingCount = 1 + level / 3;
        if (buildingCount > 5) buildingCount = 5;

        for (int i = 0; i < buildingCount; ++i) {
            int x, y;
            int attempts = 0;
            do {
                x = posDist(gen);
                y = posDist(gen);
                attempts++;
                if (attempts > 100) break;
            } while (!field.isValidPosition(x, y) || 
                        !field.getCell(x, y).isPassable() || 
                        !field.getCell(x, y).isEmpty() || 
                        std::abs(x - player.getPositionX()) + std::abs(y - player.getPositionY()) < 5);
            
            if (attempts <= 100) {
                if (i % 2 == 0) {
                    buildings.push_back(std::make_unique<EnemyTower>(x, y, 40 + level * 10, 3 + level/2, 4));
                } else {
                    buildings.push_back(std::make_unique<EnemyBuilding>(x, y, 50 + level * 15, 4));
                }
                field.getCell(x, y).setHasBuilding(true);
            }
        }
    }
    
    for (const auto& enemy : enemies) {
        if (enemy.isAlive()) {
            field.getCell(enemy.getPositionX(), enemy.getPositionY()).setHasEnemy(true);
        }
    }
    
    for (const auto& building : buildings) {
        if (!building->isDestroyed()) {
            field.getCell(building->getPositionX(), building->getPositionY()).setHasBuilding(true);
        }
    }
}