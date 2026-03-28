#pragma once

#include <random>
#include "Config.h"
#include "Field.h"
#include "objects/Enemy.h"
#include "objects/Tower.h"
#include "objects/DamageTower.h"
#include "objects/StaticObject.h"
#include "objects/SlowCell.h"

class LevelManager {
private:
    int currentLevel;
    std::mt19937 rng;

public:
    LevelManager() : currentLevel(1) {
        rng.seed(std::random_device{}());
    }

    int getCurrentLevel() const { return currentLevel; }
    void nextLevel() { currentLevel++; }
    void setLevel(int level) { currentLevel = level; }

    std::pair<int, int> getFieldSize() const {
        int baseWidth = 15;
        int baseHeight = 10;

        int width = std::min(baseWidth + (currentLevel - 1), 20);
        int height = std::min(baseHeight + (currentLevel - 1), 15);

        return {width, height};
    }

    void initializeLevel(Field& field) {
        int availableCells = (field.getWidth() - 2) * (field.getHeight() - 2) - 1; // -1 for player
        int maxObjects = static_cast<int>(availableCells * 0.7);
        
        int numEnemies = std::min(2 + currentLevel * 2, maxObjects / 2);
        int numWalls = std::min(3 + currentLevel, maxObjects / 8);
        int numTowers = std::min(1 + currentLevel / 2, 3);
        int numSlowCells = std::min(currentLevel / 2, maxObjects / 10);
        
        int totalObjects = numEnemies + numWalls + numTowers + numSlowCells;
        if (totalObjects > maxObjects) {
            float scale = static_cast<float>(maxObjects) / totalObjects;
            numEnemies = static_cast<int>(numEnemies * scale);
            numWalls = static_cast<int>(numWalls * scale);
            numSlowCells = static_cast<int>(numSlowCells * scale);
            numEnemies = std::max(numEnemies, 5);
        }

        int enemyHealthBonus = (currentLevel - 1) * 10;
        int enemyDamageBonus = (currentLevel - 1) * 3;
        
        std::uniform_int_distribution<> xDist(field.getWidth() / 2, field.getWidth() - 2);
        std::uniform_int_distribution<> yDist(1, field.getHeight() - 2);

        int placedEnemies = 0;
        int maxAttempts = numEnemies * 10;
        int attempts = 0;
        
        while (placedEnemies < numEnemies && attempts < maxAttempts) {
            int x = xDist(rng);
            int y = yDist(rng);
            attempts++;
            
            if (field.getObjectAt(x, y) == nullptr && !(x == 1 && y == 1)) {
                int health = 30 + enemyHealthBonus;
                int damage = 5 + enemyDamageBonus;
                
                field.addObject(std::make_unique<Enemy>(x, y, Config::Enemy::SPRITE, Config::Enemy::COLOR, health, damage, 1, 10));
                placedEnemies++;
            }
        }

        std::uniform_int_distribution<> wallXDist(2, field.getWidth() / 3);
        std::uniform_int_distribution<> wallYDist(2, field.getHeight() - 3);
        
        int placedWalls = 0;
        attempts = 0;
        maxAttempts = numWalls * 10;
        
        while (placedWalls < numWalls && attempts < maxAttempts) {
            int x = wallXDist(rng);
            int y = wallYDist(rng);
            attempts++;
            
            if (field.getObjectAt(x, y) == nullptr && !(x == 1 && y == 1)) {
                field.addObject(std::make_unique<Wall>(x, y));
                placedWalls++;
            }
        }

        std::uniform_int_distribution<> towerXDist(field.getWidth() * 2 / 3, field.getWidth() - 2);
        std::uniform_int_distribution<> towerYDist(2, field.getHeight() - 3);
        
        int placedTowers = 0;
        attempts = 0;
        maxAttempts = numTowers * 10;
        
        while (placedTowers < numTowers && attempts < maxAttempts) {
            int x = towerXDist(rng);
            int y = towerYDist(rng);
            attempts++;
            
            if (field.getObjectAt(x, y) == nullptr && !(x == 1 && y == 1)) {
                if (placedTowers % 2 == 0) {
                    field.addObject(std::make_unique<Tower>(x, y));
                } else {
                    field.addObject(std::make_unique<DamageTower>(x, y));
                }
                placedTowers++;
            }
        }

        std::uniform_int_distribution<> slowXDist(3, field.getWidth() / 2);
        std::uniform_int_distribution<> slowYDist(3, field.getHeight() - 4);
        
        int placedSlowCells = 0;
        attempts = 0;
        maxAttempts = numSlowCells * 10;
        
        while (placedSlowCells < numSlowCells && attempts < maxAttempts) {
            int x = slowXDist(rng);
            int y = slowYDist(rng);
            attempts++;
            
            if (field.getObjectAt(x, y) == nullptr && !(x == 1 && y == 1)) {
                field.addObject(std::make_unique<SlowCell>(x, y));
                placedSlowCells++;
            }
        }
    }

    bool isLevelCompleted(const Field& field) const {
        return !field.hasEnemies();
    }
};
