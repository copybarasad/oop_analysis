#ifndef ALLY_H
#define ALLY_H

#include "Position.h"

class Ally {
private:
    int health_;
    int maxHealth_;
    int damage_;
    Pos position_;
    bool skipNextTurn_;

public:
    Ally(int health = 40, int damage = 8);

    int getHealth() const { return health_; }
    int getMaxHealth() const { return maxHealth_; }
    bool isAlive() const { return health_ > 0; }
    void takeDamage(int amount);

    int getDamage() const { return damage_; }

    Pos getPosition() const { return position_; }
    void setPosition(int x, int y) { position_ = {x, y}; }

    bool shouldSkipTurn() const { return skipNextTurn_; }
    void skipNextMove() { skipNextTurn_ = true; }
    void resetSkipTurn() { skipNextTurn_ = false; }
};

#endif

