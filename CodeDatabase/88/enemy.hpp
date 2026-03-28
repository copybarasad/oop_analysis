#ifndef ENEMY_H
#define ENEMY_H

#include "character.hpp"

class Enemy : public Character {
    char icon = 'E';
    
public:
    Enemy(int startHealth, int startDamage, int startX, int startY);
    char getIcon() override { return icon; }
    
};

#endif