#ifndef GAME_CONFIG_H
#define GAME_CONFIG_H

struct LevelConfig {
    int levelNumber;
    int fieldWidth;
    int fieldHeight;
    int enemyHealth;
    int enemyDamage;
    int enemyCount;
    int buildingCount;
    int towerCount;
    int playerHealth;
    int playerMeleeDamage;
    int playerRangedDamage;
    int towerHealth;
    int towerDamage;
    int buildingSpawnInterval;
};

class GameConfig {
public:
    static LevelConfig getLevelConfig(int level);
    static int getMaxLevels() { return 3; }
    static int getUpgradeCost() { return 50; }
    static int getSpellRemovePercentage() { return 50; }
};

#endif