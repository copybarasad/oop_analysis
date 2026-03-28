#include "Level.h"
#include "SpellFactory.h"
#include <random>
#include <ctime>

Level::Level(int levelNumber, int fieldWidth, int fieldHeight)
    : levelNumber(levelNumber)
    , fieldWidth(fieldWidth)
    , fieldHeight(fieldHeight)
    , enemyCount(0)
    , buildingCount(0)
    , towerCount(0) {
    calculateLevelParameters();
}

void Level::calculateLevelParameters() {
    fieldWidth = 15 + (levelNumber - 1) * 3;
    fieldHeight = 15 + (levelNumber - 1) * 3;
    
    if (fieldWidth > 25) fieldWidth = 25;
    if (fieldHeight > 25) fieldHeight = 25;
    
    enemyCount = 1 + levelNumber * 2;
    buildingCount = 1 + (levelNumber - 1) / 2;
    towerCount = 1 + (levelNumber - 1) / 3;
}

bool Level::IsCompleted(const GameField& field) const {
    return field.GetEnemyCount() == 0 && 
           field.GetBuildingCount() == 0;
}

void Level::InitializeField(GameField& field) const {
    static std::mt19937 randomGenerator(static_cast<unsigned int>(std::time(nullptr)));
    
    std::uniform_int_distribution<int> xDistribution(2, fieldWidth - 3);
    std::uniform_int_distribution<int> yDistribution(2, fieldHeight - 3);
    
    int healthMultiplier = GetEnemyHealthMultiplier();
    int damageMultiplier = GetEnemyDamageMultiplier();
    
    for (int i = 0; i < enemyCount; ++i) {
        int enemyX = xDistribution(randomGenerator);
        int enemyY = yDistribution(randomGenerator);
        
        while (field.IsEnemyAt(enemyX, enemyY) || 
               field.IsBuildingAt(enemyX, enemyY) ||
               field.IsTowerAt(enemyX, enemyY) ||
               (enemyX < 3 && enemyY < 3)) {
            enemyX = xDistribution(randomGenerator);
            enemyY = yDistribution(randomGenerator);
        }
        
        Enemy enemy(50 + healthMultiplier * 10, 10 + damageMultiplier * 2);
        enemy.SetPosition(enemyX, enemyY);
        field.AddEnemy(enemy);
    }
    
    for (int i = 0; i < buildingCount; ++i) {
        int buildingX = xDistribution(randomGenerator);
        int buildingY = yDistribution(randomGenerator);
        
        while (field.IsEnemyAt(buildingX, buildingY) || 
               field.IsBuildingAt(buildingX, buildingY) ||
               field.IsTowerAt(buildingX, buildingY) ||
               (buildingX < 3 && buildingY < 3)) {
            buildingX = xDistribution(randomGenerator);
            buildingY = yDistribution(randomGenerator);
        }
        
        int spawnInterval = 5 - levelNumber / 2;
        if (spawnInterval < 2) spawnInterval = 2;
        
        int buildingHealth = 100 + healthMultiplier * 20;
        
        EnemyBuilding building(spawnInterval, buildingX, buildingY, buildingHealth);
        field.AddEnemyBuilding(building);
    }
    
    for (int i = 0; i < towerCount; ++i) {
        int towerX = xDistribution(randomGenerator);
        int towerY = yDistribution(randomGenerator);
        
        while (field.IsEnemyAt(towerX, towerY) || 
               field.IsBuildingAt(towerX, towerY) ||
               field.IsTowerAt(towerX, towerY) ||
               (towerX < 3 && towerY < 3)) {
            towerX = xDistribution(randomGenerator);
            towerY = yDistribution(randomGenerator);
        }
        
        int attackRadius = 5 + levelNumber / 2;
        int towerDamage = 15 + damageMultiplier * 3;
        int cooldown = 3 - levelNumber / 5;
        if (cooldown < 1) cooldown = 1;
        
        EnemyTower tower(towerX, towerY, attackRadius, towerDamage, cooldown);
        field.AddEnemyTower(tower);
    }
}

int Level::GetEnemyHealthMultiplier() const {
    return levelNumber - 1;
}

int Level::GetEnemyDamageMultiplier() const {
    return levelNumber - 1;
}

