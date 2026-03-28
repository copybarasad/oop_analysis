#ifndef LEVEL_MANAGER_H
#define LEVEL_MANAGER_H

struct LevelConfig {
    int levelNumber;
    int enemyCount;
    int enemyHealth;
    int enemyDamage;
};

class LevelManager {
private:
    int currentLevel;
    
public:
    LevelManager();
    
    LevelConfig getCurrentLevelConfig() const;
    void goToNextLevel();
    void setCurrentLevel(int level);
    int getCurrentLevel() const;
    
private:
    int calculateEnemyHealth() const;
    int calculateEnemyDamage() const;
    int calculateEnemyCount() const;
};

#endif