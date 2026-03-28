#ifndef ENEMY_H
#define ENEMY_H

#include "Entity.h"

class Enemy : public Entity {
public:
	Enemy();
	Enemy(int);
	bool IsFriendly() const override;
	TokenEntity serialise() const override;
};

#endif // ENEMY_H