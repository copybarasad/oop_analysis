#ifndef TEAMATE_H
#define TEAMATE_H

#include "Character.h"
#include "Enemy.h"
#include "GameField.h"
#include "Player.h"
#include <vector>

class Teamate : public Character {
private:
    int attackRange;
    
public:
    Teamate(std::string name, int health, int damage, int x, int y, int attackRange = 1);
    
    void update(GameField& field, Player& player);
    void moveTowardsEnemy(GameField& field);
    void attackEnemy(Enemy& enemy, Player& player);
    bool findNearestEnemy(GameField& field, int& targetX, int& targetY);
};

#endif