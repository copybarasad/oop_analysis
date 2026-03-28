#include "EnemyBuildingController.h"
#include "../models/events/EventNotifier.h"
#include "../models/events/EnemySpawnedEvent.h"
#include "../models/events/PrintMessageEvent.h"
#include "Enemy.h"
#include <iostream>
#include <random>

int EnemyBuildingController::calculateEnemyHealth(int level) const {
    // Базовое здоровье 50, увеличивается на 20 за каждый уровень
    return 50 + (level - 1) * 20;
}

int EnemyBuildingController::calculateEnemyDamage(int level) const {
    // Базовый урон 15, увеличивается на 5 за каждый уровень
    return 15 + (level - 1) * 5;
}

void EnemyBuildingController::processBuildingTurn() {
    if (!field) return;
    auto& buildings = field->getBuilding();
    
    int enemyHealth = calculateEnemyHealth(currentLevel);
    int enemyDamage = calculateEnemyDamage(currentLevel);
    
    for (auto& building : buildings) {
        building.update();
        if (building.canSpawnEnemy()) {
            bool placed = false;
            for (int dy = -2; dy <= 2 && !placed; ++dy) {
                for (int dx = -2; dx <= 2 && !placed; ++dx) {
                    if (dx == 0 && dy == 0) continue;
                    int nx = building.getX() + dx;
                    int ny = building.getY() + dy;
                    if (field->isValidPosition(nx, ny) && field->isPositionEmpty(nx, ny)) {
                        field->getEnemies().emplace_back(enemyHealth, enemyDamage);
                        field->getEnemies().back().setPosition(nx, ny);
                        field->getCells()[ny][nx].setHasEntity(true);
                        
                        // Уведомляем о создании врага зданием
                        EnemySpawnedEvent spawnEvent(nx, ny, "Вражеское здание");
                        EventNotifier::getInstance().notify(spawnEvent);
                        
                        PrintMessageEvent spawnMsgEvent("Вражеское здание создало нового врага!");
                        EventNotifier::getInstance().notify(spawnMsgEvent);
                        placed = true;
                    }
                }
            }
        }
    }
}

void EnemyBuildingController::generateRandomBuildings() {
    std::random_device rd;
    std::mt19937 gen(rd());
    int width = field->getWidth();
    int height = field->getHeight();

    std::uniform_int_distribution<> rndX(0, width - 1);
    std::uniform_int_distribution<> rndY(0, height - 1);
    int bx, by, tries = 0;
    do {
        bx = rndX(gen);
        by = rndY(gen);
        tries++;
        if (tries > 1000) break;
    } while (!field->isPositionEmpty(bx, by));
    if (field->isPositionEmpty(bx, by)) {
        field->getBuilding().emplace_back(bx, by);
        field->getCells()[by][bx].setHasEntity(true);
    }
}
