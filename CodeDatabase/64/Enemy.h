#ifndef ENEMY_H
#define ENEMY_H

#include "Character.h"

class Player;
class GameField;

class Enemy : public Character {
public:
    Enemy(int startX, int startY);
    
    void determineNextMove(const Player& player, const GameField& field, 
                          int& nextX, int& nextY) const;
};

#endif