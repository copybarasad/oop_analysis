#ifndef GAMELB1_ENEMY_H
#define GAMELB1_ENEMY_H

#include "Creature.h"

class Enemy : public Creature {
public:
    Enemy(int health, int damage, int x, int y);
};

#endif //GAMELB1_ENEMY_H