#ifndef LEVEL_H
#define LEVEL_H

class Level {
private:
    int levelNumber_;
    int fieldWidth_;
    int fieldHeight_;
    int enemyHP_;
    int enemyDamage_;
    int enemyCount_;
    int buildingCount_;
    int towerCount_;
    int buildingCooldown_;

public:
    Level(int levelNum, int width, int height, int enemyHP, int enemyDmg, 
          int enemyCount, int buildingCount, int towerCount, int buildingCooldown);
    
    static Level generateLevel(int levelNum);
    
    int getLevelNumber() const { return levelNumber_; }
    int getFieldWidth() const { return fieldWidth_; }
    int getFieldHeight() const { return fieldHeight_; }
    int getEnemyHP() const { return enemyHP_; }
    int getEnemyDamage() const { return enemyDamage_; }
    int getEnemyCount() const { return enemyCount_; }
    int getBuildingCount() const { return buildingCount_; }
    int getTowerCount() const { return towerCount_; }
    int getBuildingCooldown() const { return buildingCooldown_; }
};

#endif

