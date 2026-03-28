#ifndef ENEMY_H
#define ENEMY_H
#include "Character.h"

class Enemy: public Character{
public:
    Enemy(int hp, int damage, std::pair<int, int> pos);
    void step(std::pair<int, int> delta);
    
    bool shouldSkipTurn();
    void setSkipNextTurn(bool v);

private:
    bool skipNextTurn_;
};

#endif