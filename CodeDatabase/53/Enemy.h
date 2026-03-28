#ifndef ENEMY_H
#define ENEMY_H

#include <string>
#include "Position.h"
#include "Character.h"

class Player;
class GameField;

class Enemy : public Character {

public:
    Enemy(std::string enemyName, int startHealth, int startDamage, int StartX, int StartY);
    
    void move(int playerX, int playerY, Player& player, GameField& field);
    void displayInfo() const;
};

#endif