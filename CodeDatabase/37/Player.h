#ifndef PLAYER_H
#define PLAYER_H

#include "Position.h"

class Player {
private:
    int health;
    int meleeDamage;
    int rangedDamage;
    bool isRangedMode;
    Position position;
    bool canMove;

public:
    Player(int startHealth, int startMeleeDamage, int startRangedDamage);
    
    // Геттеры
    int getHealth() const { return health; }
    int getDamage() const { return isRangedMode ? rangedDamage : meleeDamage; }
    Position getPosition() const { return position; }
    bool getCanMove() const { return canMove; }
    bool isAlive() const { return health > 0; }
    bool getCombatMode() const { return isRangedMode; } // ADD THIS LINE
    
    void move(const Position& newPos);
    void switchCombatMode();
    void takeDamage(int amount);
    void setCanMove(bool move) { canMove = move; }
    void setPosition(const Position& pos) { position = pos; }
};

#endif