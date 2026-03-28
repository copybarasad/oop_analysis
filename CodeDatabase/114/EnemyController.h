#ifndef ENEMYCONTROLLER_H
#define ENEMYCONTROLLER_H

#include "interfaces/IEnemyField.h"

class EnemyController {
private:
    IEnemyField* field;
    int currentLevel;
    
    int calculateEnemyHealth(int level) const;
    int calculateEnemyDamage(int level) const;
    
public:
    EnemyController(IEnemyField* field) : field(field), currentLevel(1) {}
    void processEnemyTurn();
    void generateRandomEnemies();
    void setCurrentLevel(int level) { currentLevel = level; }
    int getCurrentLevel() const { return currentLevel; }
};

#endif
