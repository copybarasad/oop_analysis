#ifndef LEVEL_H
#define LEVEL_H

#include "GameField.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemyBuilding.h"
#include "EnemyTower.h"
#include <memory>

class Level {
public:
    Level(int levelNumber, int fieldWidth, int fieldHeight);
    
    int GetLevelNumber() const { return levelNumber; }
    int GetFieldWidth() const { return fieldWidth; }
    int GetFieldHeight() const { return fieldHeight; }
    int GetEnemyCount() const { return enemyCount; }
    int GetBuildingCount() const { return buildingCount; }
    int GetTowerCount() const { return towerCount; }
    
    bool IsCompleted(const GameField& field) const;
    
    void InitializeField(GameField& field) const;
    
    int GetEnemyHealthMultiplier() const;
    int GetEnemyDamageMultiplier() const;
    
private:
    int levelNumber;
    int fieldWidth;
    int fieldHeight;
    int enemyCount;
    int buildingCount;
    int towerCount;
    
    void calculateLevelParameters();
};

#endif

