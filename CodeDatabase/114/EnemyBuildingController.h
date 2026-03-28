#ifndef ENEMYBUILDINGCONTROLLER_H
#define ENEMYBUILDINGCONTROLLER_H

#include "interfaces/IEnemyBuildingField.h"

class EnemyBuildingController {
private:
    IEnemyBuildingField* field;
    int currentLevel;
    
    int calculateEnemyHealth(int level) const;
    int calculateEnemyDamage(int level) const;
    
public:
    EnemyBuildingController(IEnemyBuildingField* field) : field(field), currentLevel(1) {}
    void processBuildingTurn();
    void generateRandomBuildings();
    void setCurrentLevel(int level) { currentLevel = level; }
    int getCurrentLevel() const { return currentLevel; }
};

#endif
