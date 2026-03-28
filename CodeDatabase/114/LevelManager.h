#ifndef LEVELMANAGER_H
#define LEVELMANAGER_H

#include "GameField.h"
#include "Player.h"
#include <memory>

class EnemyController;
class EnemyBuildingController;

class LevelManager {
private:
    std::unique_ptr<GameField> field;
    int currentLevel;
    
    void generateRandomField();
    std::pair<int, int> getLevelDimensions(int level) const;
    void transferPlayerState(Player& from, Player& to, bool upgradeHealth, bool upgradeDamage);
    
public:
    LevelManager();
    ~LevelManager() = default;
    
    void initializeLevel(int level);
    void nextLevel(bool upgradeHealth = false, bool upgradeDamage = false);
    
    GameField& getField() { return *field; }
    const GameField& getField() const { return *field; }
    int getCurrentLevel() const { return currentLevel; }
    void setCurrentLevel(int level) { currentLevel = level; }
    void setField(std::unique_ptr<GameField> newField) { field = std::move(newField); }
};

#endif

