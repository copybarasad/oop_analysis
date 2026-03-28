#ifndef GAMELEVEL_H
#define GAMELEVEL_H

class GameLevel {
private:
    int levelNumber;
    int mapWidth;
    int mapHeight;
    int enemyCount;
    int towerCount;
    int enemyHealth;
    int enemyDamage;
    int towerHealth;
    int towerDamage;
    float spellCostMultiplier;

public:
    GameLevel(int level);
    
    // Геттеры для всех полей
    int getLevelNumber() const { return levelNumber; }
    int getMapWidth() const { return mapWidth; }
    int getMapHeight() const { return mapHeight; }
    int getEnemyCount() const { return enemyCount; }
    int getTowerCount() const { return towerCount; }
    int getEnemyHealth() const { return enemyHealth; }
    int getEnemyDamage() const { return enemyDamage; }
    int getTowerHealth() const { return towerHealth; }
    int getTowerDamage() const { return towerDamage; }
    float getSpellCostMultiplier() const { return spellCostMultiplier; }
};

#endif