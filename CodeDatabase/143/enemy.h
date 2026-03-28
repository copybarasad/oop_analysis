#ifndef ENEMY_H
#define ENEMY_H

#include "game_object.h"
#include "game_constants.h"

class Enemy : public GameObject {
public:
    Enemy(const Position& position);
    bool canAttackPlayer(const Position& playerPosition) const;
};


#endif



