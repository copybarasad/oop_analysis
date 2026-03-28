#ifndef ALLYUNIT_H
#define ALLYUNIT_H

#include "position.h"

class Player;
class GameField;

class AllyUnit {
private:
    Position position;
    int health;
    int damage;
    bool alive;
    
public:
    AllyUnit(Position pos = Position(0,0), int health = 40, int damage = 15);
    
    void takeTurn(Player* player, GameField* field);
    void takeDamage(int amount);
    bool isAlive() const;
    Position getPosition() const;
    void moveTowardEnemy(GameField* field);
};

#endif