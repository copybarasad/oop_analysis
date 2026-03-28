#include "level_manager.h"
#include "enemy.h"
#include "tower.h"
#include "building.h"
#include <random>
#include <iostream>

LevelManager::LevelManager(int startLevel) : currentLevel_(startLevel) {}

int LevelManager::getCurrentLevel() const {
    return currentLevel_;
}

void LevelManager::nextLevel() {
    currentLevel_++;
}

LevelManager::LevelParams LevelManager::getCurrentLevelParams() const {
    return getLevelParams(currentLevel_);
}

LevelManager::LevelParams LevelManager::getLevelParams(int level) const {
    LevelParams params;

    params.fieldRows = 12 + (level - 1) * 2;
    params.fieldCols = 12 + (level - 1) * 2;
    params.enemyCount = 3 + (level - 1) * 2;
    params.buildingCount = 2 + (level - 1);
    params.enemyHealth = 5 + (level - 1) * 2;
    params.enemyDamage = 2 + (level - 1);
    params.buildingHealth = 10 + (level - 1) * 5;

    if (params.fieldRows > 30) params.fieldRows = 30;
    if (params.fieldCols > 30) params.fieldCols = 30;
    
    return params;
}

void LevelManager::initializeLevel(Field& field, Player* player) {
    if (!player) return;
    
    auto params = getCurrentLevelParams();

    player->setHealth(50 + (currentLevel_ - 1) * 15);

    Hand& hand = player->hand();
    size_t spellsToRemove = hand.size() / 2;
    
    std::random_device rd;
    std::mt19937 gen(rd());
    
    std::cout << "Половина карточек заклинаний удалена при переходе на новый уровень\n";

    int r = params.fieldRows / 2;
    int c = params.fieldCols / 2;
    
    if (field.placePlayer(player, r, c)) {
        std::cout << "Игрок помещен на новый уровень\n";
    }

    std::uniform_int_distribution<int> rowDist(0, params.fieldRows - 1);
    std::uniform_int_distribution<int> colDist(0, params.fieldCols - 1);
    
    for (int i = 0; i < params.enemyCount; ++i) {
        int rr, cc;
        do {
            rr = rowDist(gen);
            cc = colDist(gen);
        } while ((rr == r && cc == c) || !field.cellIsEmpty(rr, cc));
        
        Enemy* enemy = new Enemy(
            field.generateEntityId(),
            params.enemyHealth,
            params.enemyDamage
        );
        field.placeEnemy(enemy, rr, cc);
    }

    for (int i = 0; i < params.buildingCount; ++i) {
        int rr, cc;
        do {
            rr = rowDist(gen);
            cc = colDist(gen);
        } while ((rr == r && cc == c) || !field.cellIsEmpty(rr, cc));
        
        Building* building = new Building(
            field.generateEntityId(),
            params.buildingHealth,
            15
        );
        field.placeBuilding(building, rr, cc);
    }
}
