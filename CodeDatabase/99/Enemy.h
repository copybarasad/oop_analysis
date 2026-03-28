#ifndef ENEMY_H
#define ENEMY_H

#include "Unit.h"

// Enemy class - represents computer-controlled hostile characters
class Enemy : public Unit {
public:
	Enemy(int hp, int damage);
};

#endif // ENEMY_H
