#ifndef MOVINGENEMY_H
#define MOVINGENEMY_H

#include "position.h"

class GameField;
class Player;

class MovingEnemy {
private:
    Position position;
    int health;
    int damage;
    bool alive;
    
public:
    MovingEnemy(Position pos = Position(0,0), int health = 30, int damage = 10);
    
    void takeTurn(Player* player, GameField* field);  // Moves OR attacks
    void takeDamage(int amount);
    bool isAlive() const;
    Position getPosition() const;
    void setPosition(const Position& pos);
    
    // Movement helper
    void moveToward(Position playerPos, GameField* field);
    int getHealth() const {
        return health;
    }
};

#endif