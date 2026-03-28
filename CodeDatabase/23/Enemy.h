#ifndef ENEMY_H
#define ENEMY_H

#include "Entity.h"
#include <string>

class GameField;
class Player;

class Enemy : public Entity {
private:
    std::string name;
    int dmg;
    int trapImmuneTurns = 0;

public:
    Enemy(int x, int y, const std::string& name, int hp, int dmg);
    
    std::string getName() const;
    int getHp() const;
    int getDamage() const;
    void moveTowardsPlayer(int player_x, int player_y, GameField& field);
    bool attackPlayer(Player* player);
    void setTrapImmune(int t) { trapImmuneTurns = t; }
};

#endif 
