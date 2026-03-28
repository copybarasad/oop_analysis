#include "GameConfig.h"

LevelConfig GameConfig::getLevelConfig(int level) {
    LevelConfig config;
    config.levelNumber = level;
    
    switch(level) {
        case 1:
            config.fieldWidth = 20;
            config.fieldHeight = 20;
            config.enemyHealth = 20;
            config.enemyDamage = 2;
            config.enemyCount = 3;
            config.buildingCount = 1;
            config.towerCount = 1;
            config.playerHealth = 100;
            config.playerMeleeDamage = 20;
            config.playerRangedDamage = 15;
            config.towerHealth = 50;
            config.towerDamage = 10;
            config.buildingSpawnInterval = 5;
            break;
            
        case 2:
            config.fieldWidth = 22;
            config.fieldHeight = 22;
            config.enemyHealth = 30;
            config.enemyDamage = 3;
            config.enemyCount = 5;
            config.buildingCount = 2;
            config.towerCount = 2;
            config.playerHealth = 120;
            config.playerMeleeDamage = 25;
            config.playerRangedDamage = 18;
            config.towerHealth = 75;
            config.towerDamage = 15;
            config.buildingSpawnInterval = 4;
            break;
            
        case 3:
            config.fieldWidth = 25;
            config.fieldHeight = 25;
            config.enemyHealth = 40;
            config.enemyDamage = 5;
            config.enemyCount = 8;
            config.buildingCount = 3;
            config.towerCount = 3;
            config.playerHealth = 150;
            config.playerMeleeDamage = 30;
            config.playerRangedDamage = 22;
            config.towerHealth = 100;
            config.towerDamage = 20;
            config.buildingSpawnInterval = 3;
            break;
            
        default:
            config = getLevelConfig(3);
    }
    
    return config;
}