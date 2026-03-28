#ifndef ENEMY
#define ENEMY

#include "Character.h"

class Enemy : public Character
{
public:
    Enemy(const Enemy &e);

    Enemy(int maxLife = 20, int meleeDamage = 10, int longRangeDamage = 2);

    // Enemy &operator=(const Enemy &obj);
};

#endif