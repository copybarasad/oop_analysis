#ifndef ENEMY_H
#define ENEMY_H

#include "Creature.h" 

class Game;

class Enemy: public Creature{
public:
    
    Enemy();
    friend class Game;
};

#endif