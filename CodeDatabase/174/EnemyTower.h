#ifndef ENEMY_TOWER_H
#define ENEMY_TOWER_H

#include "EnemyBuilding.h"
#include "Position.h" 

class Player;
class GameField;

class EnemyTower : public EnemyBuilding {
    int attackRange;
    int attackCooldown;
    int currentCooldown;

public:
    EnemyTower(int x, int y, int range, int damage, int cooldown);
    void update();
    void tryAttack(Player& player, GameField& field); 
    CellType getCellType() const;
};

#endif